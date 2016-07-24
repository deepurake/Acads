#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <string.h> /* memset */
#include <unistd.h> /* close */
#include <omp.h>
#include <sys/time.h>

#define debug 0
int main(int argc, char **argv)
{
	// myID = Currenr processor ID, numProc = total number of processes, numLoops = Total number of loops in each barrier we have to run (log(numProc))
	int myId, numProc, numLoops;
	// Number of barriers we are calling (default = 4)
	int numBarr = 4;
	// Number of Threads we are initializing in each process
	int numThreads = 3;

	if(argc >= 2)
	{
		numBarr = atoi(argv[1]);
	}
	if(argc >= 3)
	{
		numThreads = atoi(argv[2]);
	}

	// Initialize MPI
	MPI_Init(&argc, &argv);

	// Determine number of processes and the rank of the process which is the process ID
	MPI_Comm_size(MPI_COMM_WORLD, &numProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &myId);

	// Calculare number of Dissemmination rounds we have to run (logarithmic rounds)
	numLoops = ceil(log(numProc)/log(2))-1;

	// Profiling code
	struct timeval start, end;

	if(myId == 0)
	{
		printf("ENTER,Combined Barrier, my id: %d, num_threads: %d\n", myId, numProc);
	}

	// Data for tournament barriers
	// num of threads which contend in tree barrier except in ground level(to take care if logn is non integers)
	int numPairs = pow(2,numLoops); 
	int numSpill = numProc - numPairs;

	MPI_Status status;

	// Data Struct for Sense reversal algorithm
	omp_set_num_threads(numThreads);
	int num_threads = numThreads;
	int count = num_threads;
	int sense = 1;
	// Initialize OMP threads
#pragma omp parallel
	{
		// run nnumBarr barriers before terminating
		int barrNum = 0;
		int thread_num =  omp_get_thread_num();// local variables for barrier
		int local_sense = 1;
		int mycount = 0;
		if(thread_num == 0)
		{
			// Profiling code
			gettimeofday(&start, NULL);
		}
		// Run Combined barrier for numBarr times
		for (; barrNum < numBarr; barrNum++)
		{
			int next = myId-numPairs;
			int out_buf[1] = {barrNum};
			int in_buf[1] = {-1};
			int loop = 0;

			// SENSE REVERSAL Algorithm 
			// reverse local sense
			local_sense = 1-local_sense;
#pragma omp critical 
			{
				mycount = count--;
			}

			if(mycount == 1)
			{
				// Last thread to arrive
				count = num_threads;
				sense = local_sense;
			}
			else
			{
				// All the threads to arrive early wait till sense is reversed
				while(sense != local_sense);
			}

			// TOURNAMENT BARRIER Algorithm
			if(thread_num == 0)
			{
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
					if(myId < numSpill)
					{
						MPI_Recv(in_buf,1,MPI_INT,myId+numPairs,barrNum+1,MPI_COMM_WORLD, &status);
#if debug
						printf("<====recvd from %d myID: %d\n",myId+numPairs,myId);
						fflush(stdout);
#endif
					}

					// Actual tournament algorithm here
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
			}

			// SENSE REVERSAL algorithm 
			// reverse local sense
			local_sense = 1-local_sense;
#pragma omp critical 
			{
				mycount = count--;
			}
			if(mycount == 1)
			{
				// Last thread to arrive
				count = num_threads;
				sense = local_sense;
			}
			else
			{
				// All the threads to arrive early wait till sense is reversed
				while(sense != local_sense);
			}
			//printf("After barrier %d thread: %d %d time:%d\n",barrNum,myId,thread_num,(int)end.tv_usec);
		}
		if(thread_num == 0)
		{
			// Profiling code
			gettimeofday(&end, NULL);
		}
	}
	//printf("Exiting thread: %d\n",myId);

	if(myId == 0)
	{
		// Output the statistics collection
		printf("EXIT,Combined Barrier, time: %d, num_proc: %d, num_loops: %d num_threads: %d\n", (int)((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec)), numProc, numBarr,numThreads);
	}
	MPI_Finalize();
	return 0;
}

