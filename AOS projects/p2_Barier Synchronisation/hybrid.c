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
	int myId, numProc, numLoops;
	int numBarr = 4;
	int numThreads = 3;



	if(argc >= 2)
	{
		numBarr = atoi(argv[1]);
	}
	if(argc >= 3)
	{
		numThreads = atoi(argv[2]);
	}

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &numProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &myId);
	numLoops = ceil(log(numProc)/log(2))-1;
	struct timeval start, end;

	gettimeofday(&start, NULL);
	printf("Enter,Combined Barrier, my id: %d, num_threads: %d\n", myId, numProc);


	// Data for tournament barriers
	// num of threads which contend in tree barrier except in ground level(to take care if logn is non integers)
	int numPairs = pow(2,numLoops); 
	int numSpill = numProc - numPairs;
	//printf("pairs: %d , spill %d\n",numPairs,numSpill);

	MPI_Status status;

	omp_set_num_threads(numThreads);
	int num_threads = numThreads;
	int count = num_threads;
	int sense = 1;
	#pragma omp parallel
	{
		// run nnumBarr barriers before terminating
		int barrNum = 0;
		int thread_num =  omp_get_thread_num();// local variables for barrier
		int local_sense = 1;
		int mycount = 0;
		for (; barrNum < numBarr; barrNum++)
		{
			int next = myId-numPairs;
			int out_buf[1] = {barrNum};
			int in_buf[1] = {-1};
			int loop = 0;

			// reverse local sense
			local_sense = 1-local_sense;
#pragma omp critical 
			{
				mycount = count--;
			}

			if(mycount == 1)
			{
				count = num_threads;
				sense = local_sense;
			}
			else
			{
				while(sense != local_sense);
			}

			if(thread_num == 0)
			{
				if(myId >= numPairs)
				{
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

			// reverse local sense
			local_sense = 1-local_sense;
#pragma omp critical 
			{
				//qwertycount = count -1;
				mycount = count--;
			}

			if(mycount == 1)
			{
				count = num_threads;
				sense = local_sense;
			}
			else
			{
				while(sense != local_sense);
			}

			//gettimeofday(&end, NULL);
			//MPI_Barrier(MPI_COMM_WORLD);
			//printf("After barrier %d thread: %d %d time:%d\n",barrNum,myId,thread_num,(int)end.tv_usec);
		}
	}
	//printf("Exiting thread: %d\n",myId);
	gettimeofday(&end, NULL);

	printf("EXIT,Combined Barrier, time: %d, num_proc: %d, num_loops: %d\n", (int)((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec)), numProc, numBarr);
	MPI_Finalize();
	return 0;
}

