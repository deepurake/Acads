#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <string.h> /* memset */
#include <unistd.h> /* close */
#include <sys/time.h>


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
	numLoops = ceil(log(numProc)/log(2));
	if(myId == 0)
	{
		printf("START,Dissemination barrier, my id: %d, num_threads: %d\n", myId, numProc);
	}

	// Profiling code
	struct timeval start, end;
	gettimeofday(&start, NULL);

	// run nnumBarr barriers before terminating
	int barrNum = 0;
	// Sense which will be reversed after every barrier to take care of race conditions
	int sense = 0;
	for (; barrNum < numBarr; barrNum++)
	{
		int loop = 0;
		int next = myId + pow(2,loop);
		int prev = myId - pow(2,loop);
		int rcvd[numLoops];
		memset(rcvd,sense, numLoops*sizeof(int));
		sense = 1-sense;
		MPI_Status status;
		// Barrier Algorithm implementation
		for(; loop < numLoops+1; loop++)
		{
			// the next node increases exponentially every time (to make it log(N) rounds), so does previous node
			next= (myId + (int)pow(2,loop))%numProc;
			prev= ((myId - (int)pow(2,loop))%numProc+numProc) % numProc  ;
			int out_buf[1] = {loop};
			int in_buf[1] = {-1};
			// Send message to next node and wait from previous node
			MPI_Send(out_buf, 1, MPI_INT, next, loop+1, MPI_COMM_WORLD);
			while(rcvd[loop] != sense)
			{
				MPI_Recv(in_buf,1,MPI_INT, prev, loop+1,MPI_COMM_WORLD, &status);
				rcvd[in_buf[0]] = sense;
			}
		}
		//printf("After barrier %d thread: %d\n",barrNum,myId);
		//fflush(stdout);
	}
	//printf("Exiting thread: %d\n",myId);

	// Calculate the time taken
	gettimeofday(&end, NULL);
	if(myId == 0)
	{
		printf("EXIT,Dissemination Barrier, time: %d num_proc: %d num_loops: %d\n", (int)((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec)), numProc, numBarr);
	}
	MPI_Finalize();
	return 0;
}

