#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <string.h> /* memset */
#include <unistd.h> /* close */
#include <sys/time.h>

#define debug 0

int main(int argc, char **argv)
{
	// myID = Currenr processor ID, numProc = total number of processes, numLoops = Total number of loops in each barrier we have to run (log(numProc))
	int myId, numProc, numLoops;
	// Number of barriers we are calling (default = 4)
	int numBarr = 4;
	if(argc == 2)
	{
		numBarr = atoi(argv[1]);
	}

	// Initialize MPI
	MPI_Init(&argc, &argv);

	// Determine number of processes and the rank of the process which is the process ID
	MPI_Comm_size(MPI_COMM_WORLD, &numProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &myId);

	// Calculare number of Dissemmination rounds we have to run (logarithmic rounds)
	numLoops = ceil(log(numProc)/log(2))-1;

	if(myId == 0)
	{
		printf("START,Tournament Barrier, my id: %d, num_threads: %d\n", myId, numProc);
	}

	// Data for tournament barriers
	// num of threads which contend in tree barrier except in ground level(to take care if logn is non integers)
	int numPairs = pow(2,numLoops); 
	int numSpill = numProc - numPairs;
#if debug
	printf("pairs: %d , spill %d\n",numPairs,numSpill);
#endif

	// run nnumBarr barriers before terminating
	int barrNum = 0;

	// Profiling code
	struct timeval start, end;
	gettimeofday(&start, NULL);

	// Run Tournament Barrier for numBarr number of times 
	MPI_Status status;
	for (; barrNum < numBarr; barrNum++)
	{
		int next = myId-numPairs;
		int out_buf[1] = {barrNum};
		int in_buf[1] = {-1};
		if(myId >= numPairs)
		{
			// To take care of spill nodes(if logN is not integer, what ever nodes in addition to power of 2)
			// which ever is greater than log(N) nodes
			// send message to Winner and wait for it to wake up
			MPI_Send(out_buf, 1, MPI_INT, next, barrNum+1, MPI_COMM_WORLD);
#if debug
			printf("====>Sent message to %d myID: %d\n",next,myId);
			fflush(stdout);
#endif
			MPI_Recv(in_buf, 1, MPI_INT, next, barrNum+1, MPI_COMM_WORLD, &status);
#if debug
			printf("<====recvd from %d myID: %d\n",next,myId);
			fflush(stdout);
#endif
		}
		else
		{
			// first half nodes enter second round(actual tournament Algorithm), wait for loosers in the first round(spill Nodes) to send message
			int loop = 0;
			if(myId < numSpill)
			{
				MPI_Recv(in_buf,1,MPI_INT,myId+numPairs,barrNum+1,MPI_COMM_WORLD, &status);
#if debug
				printf("<====recvd from %d myID: %d\n",myId+numPairs,myId);
				fflush(stdout);
#endif
			}

			//Arrival tree
			for(loop = 1; loop <= numLoops; loop++)
			{
				int inc = (int)pow(2,loop-1);
				if(myId%(2*inc) == 0)
				{
					next = myId+inc;
					in_buf[0] = -1;
					MPI_Recv(in_buf, 1, MPI_INT, next, barrNum+1, MPI_COMM_WORLD, &status);
#if debug
					printf("<====recvd from %d myID: %d\n",myId+numPairs,myId);
					fflush(stdout);
#endif
				}
				else
				{
					next = myId-inc;
					in_buf[0] = -1;
#if debug
					printf("====>sending message to %d myID: %d\n",next,myId);
					fflush(stdout);
#endif
					MPI_Send(out_buf, 1, MPI_INT, next, barrNum+1, MPI_COMM_WORLD);
					MPI_Recv(in_buf, 1, MPI_INT, next, barrNum+1, MPI_COMM_WORLD, &status);
					break;
				}
			}

			// wakeup tree
			for(loop = numLoops; loop > 0; loop--)
			{
				int inc = (int)pow(2,loop-1);
				if(myId%inc == 0)
				{
					next = myId+inc;
#if debug
					printf("====>sending message to %d myID: %d\n",next,myId);
					fflush(stdout);
#endif
					MPI_Send(out_buf, 1, MPI_INT, next, barrNum+1, MPI_COMM_WORLD);
				}
			}		

			// Send message to spill nodes(losers in first round) to wake up
			if(myId < numSpill)
			{
#if debug
				printf("====>sending message to %d myID: %d\n",myId+numPairs,myId);
				fflush(stdout);
#endif
				MPI_Send(out_buf,1,MPI_INT,myId+numPairs,barrNum+1,MPI_COMM_WORLD);
			}
		}
		//printf("After barrier %d thread: %d\n",barrNum,myId);
		//fflush(stdout);
	}
	//printf("Exiting thread: %d\n",myId);

	// Profiling code
	gettimeofday(&end, NULL);
	if(myId == 0)
	{
		printf("EXIT,Tournament Barrier, time: %d, num_proc: %d, num_loops: %d\n", (int)((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec)), numProc, numBarr);
	}

	MPI_Finalize();
	return 0;
}

