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
  int myId, numProc, numLoops;
  int numBarr = 4;
  if(argc == 2)
  {
  	numBarr = atoi(argv[1]);
  }
  
  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &numProc);
  MPI_Comm_rank(MPI_COMM_WORLD, &myId);
	numLoops = ceil(log(numProc)/log(2))-1;
	 struct timeval start, end;

  gettimeofday(&start, NULL);
        printf("START,Tournament Barrier, my id: %d, num_threads: %d\n", myId, numProc);


	// Data for tournament barriers
	// num of threads which contend in tree barrier except in ground level(to take care if logn is non integers)
	int numPairs = pow(2,numLoops); 
	int numSpill = numProc - numPairs;
	printf("paris: %d , spill %d\n",numPairs,numSpill);
	
	// run nnumBarr barriers before terminating
	int barrNum = 0;
  
  MPI_Status status;
	for (; barrNum < numBarr; barrNum++)
	{
		int next = myId-numPairs;
		int out_buf[1] = {barrNum};
		int in_buf[1] = {-1};
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
			int loop = 0;
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
					//in_buf[0] = -1;
#if debug
					printf("====>sending message to %d myID: %d\n",next,myId);
					fflush(stdout);
#endif
					MPI_Send(out_buf, 1, MPI_INT, next, barrNum+1, MPI_COMM_WORLD);
					//MPI_Recv(in_buf, 1, MPI_INT, next, barrNum+1, MPI_COMM_WORLD, &status);
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
		//MPI_Barrier(MPI_COMM_WORLD);
		//printf("After barrier %d thread: %d\n",barrNum,myId);
		//fflush(stdout);
	}
	//printf("Exiting thread: %d\n",myId);
	
  gettimeofday(&end, NULL);
  printf("EXIT,Tournament Barrier, time: %d, num_proc: %d, num_loops: %d\n", (int)((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec)), numProc, numBarr);


  MPI_Finalize();
  return 0;
}

