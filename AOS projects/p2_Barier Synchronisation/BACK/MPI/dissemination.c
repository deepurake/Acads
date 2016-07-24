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
  int myId, numProc, numLoops;
  int numBarr = 4;
  if(argc == 2)
  {
  	numBarr = atoi(argv[1]);
  }
 
  struct timeval start, end;

  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &numProc);
  MPI_Comm_rank(MPI_COMM_WORLD, &myId);
	numLoops = ceil(log(numProc)/log(2));
	if(myId == 0)
		printf("START,Dissemination barrier, my id: %d, num_threads: %d\n", myId, numProc);
	
	gettimeofday(&start, NULL);
	// run nnumBarr barriers before terminating
	int barrNum = 0;
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
		for(; loop < numLoops+1; loop++)
		{
			next= (myId + (int)pow(2,loop))%numProc;
			prev= ((myId - (int)pow(2,loop))%numProc+numProc) % numProc  ;
			int out_buf[1] = {loop};
			int in_buf[1] = {-1};
			MPI_Send(out_buf, 1, MPI_INT, next, loop+1, MPI_COMM_WORLD);
			while(rcvd[loop] != sense)
			{
				MPI_Recv(in_buf,1,MPI_INT, prev, loop+1,MPI_COMM_WORLD, &status);
				rcvd[in_buf[0]] = sense;
				if(myId == 0)
				{
					//sleep(1);
				}
			}
		}
		//printf("After barrier %d thread: %d\n",barrNum,myId);
		//fflush(stdout);
	}
	//printf("Exiting thread: %d\n",myId);

  gettimeofday(&end, NULL);
	if(myId == 0)
        	printf("EXIT,Dissemination Barrier, time: %d num_proc: %d num_loops: %d\n", (int)((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec)), numProc, numBarr);
  MPI_Finalize();
  return 0;
}

