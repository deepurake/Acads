#include <sys/time.h>
#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <string.h> /* memset */
#include <unistd.h> /* close */

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
		printf("START,MPI Barrier, my id: %d num_threads: %d \n", myId, numProc);

	gettimeofday(&start, NULL);	
	// run nnumBarr barriers before terminating
	int barrNum = 0;
	for (; barrNum < numBarr; barrNum++)
	{
		MPI_Barrier(MPI_COMM_WORLD);
		//struct timeval start;//, end;
		//gettimeofday(&start,NULL);
		//printf("After barrier %d thread: %d time: %d\n",barrNum,myId,(int)start.tv_usec);
		//fflush(stdout);
	}
	//printf("Exiting thread: %d\n",myId);
	
  MPI_Finalize();
  gettimeofday(&end, NULL);
  
  if(myId == 0)printf("EXIT,MPI Barrier, time: %d, num_proc: %d, num_loops: %d\n", (int)((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec)), numProc, numBarr);
	return 0;
}

