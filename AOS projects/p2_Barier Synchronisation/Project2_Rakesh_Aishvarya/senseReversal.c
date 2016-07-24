#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main(int argc, char **argv)
{
	// Initialize number of threads and number of barrier variables
	int num_threads = 3;
	int num_barriers = 3;
	if(argc == 3)
	{
		num_threads = atoi(argv[1]);
		num_barriers = atoi(argv[2]);
	}

	// Profiling code
	struct timeval start, end;

	printf("START, SenseReversal Barrier, num_threads: %d, num_barriers %d\n", num_threads,num_barriers);

	// Globabl barrier variables
	int count = num_threads;
	int sense = 1;

	// intialize threads
	omp_set_num_threads(num_threads);
#pragma omp parallel
	{
		// local variables for barrier
		int local_sense = 1;
		int mycount = 0;
		int thread_num = omp_get_thread_num();
		int idx = 0;

		// Profiling code
		if(thread_num == 0)
		{
			gettimeofday(&start, NULL);
		}
		// Run sense reversal barriers multiple times
		for (idx = 0; idx<num_barriers; idx++)
		{
			// reverse local sense
			local_sense = 1-local_sense;
#pragma omp critical 
			{
				mycount = count--;
			}

			if(mycount == 1)
			{
				// last thread to arrive sets the sense
				count = num_threads;
				sense = local_sense;
			}
			else
			{
				// All the threads arriving early waits for the sense to be reversed
				while(sense != local_sense);
			}

			//printf("After barrier %d thread %d\n",idx+1, thread_num);
		}

		// Profiling code
		if(thread_num == 0)
		{
			gettimeofday(&end, NULL);
		}

		// The code for the barrier
		//printf("Exiting the thread %d\n", thread_num);
	} 

	printf("EXIT,SenseReversal Barrier, time: %d, num_proc: %d, num_loops: %d\n", (int)((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec)), num_threads, num_barriers);
	return 0;
}

