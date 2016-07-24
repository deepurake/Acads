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

	// Initialize MPI
	MPI_Init(&argc, &argv);

	// Determine number of processes and the rank of the process which is the process ID
	MPI_Comm_size(MPI_COMM_WORLD, &numProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &myId);

	// Calculare number of Dissemmination rounds we have to run (logarithmic rounds)
	numLoops = ceil(log(numProc)/log(2));
	if(myId == 0)
	{
		printf("START,MPI Barrier, my id: %d num_threads: %d \n", myId, numProc);
	}

	// Collect stats for profiling
	struct timeval start, end;
	gettimeofday(&start, NULL);	

	// run numBarr barriers before terminating
	int barrNum = 0;
	for (; barrNum < numBarr; barrNum++)
	{
		MPI_Barrier(MPI_COMM_WORLD);
		//printf("After barrier %d thread: %d time: %d\n",barrNum,myId,(int)start.tv_usec);
		//fflush(stdout);
	}
	//printf("Exiting thread: %d\n",myId);

	MPI_Finalize();

	// Collect stats for profiling
	gettimeofday(&end, NULL);
	// Since all the processess arrive here at the same time, we print stats only for Process 0 which should be almost same for all the processes
	if(myId == 0)
	{
		printf("EXIT,MPI Barrier, time: %d, num_proc: %d, num_loops: %d\n", (int)((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec)), numProc, numBarr);
	}

	return 0;
}

