#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main(int argc, char **argv)
{
	int num_threads = 3;
	int num_barriers = 3;
  // Serial code
  struct timeval start, end;

  gettimeofday(&start, NULL);

 // printf("OpenMP barrier implementation \n");
	if(argc == 3)
	{
		num_threads = atoi(argv[1]);
		num_barriers = atoi(argv[2]);
	}
        printf("START,OpenMP Barrier, num_threads: %d, num_barriers %d\n", num_threads,num_barriers);
	//printf("num_hreads %d num_barriers %d\n",num_threads,num_barriers);
  omp_set_num_threads(num_threads);

#pragma omp parallel
  {
    // Now we're in the parallel section
    //int num_threads = omp_get_num_threads();
    int thread_num = omp_get_thread_num();
    int idx = 0;
    printf("Entered the thread %d\n",thread_num);
    for (idx = 0; idx<num_barriers; idx++)
    {
 #pragma omp barrier
    	//printf("After barrier %d thread %d\n",idx+1, thread_num);
    }
    
    // The code for the barrier
    //printf("Exiting the thread %d\n", thread_num);
  } 

  // Resume serial code
  //printf("Program completed\n");
  gettimeofday(&end, NULL);

        printf("EXIT,OpenMP Barrier, time: %d, num_proc: %d, num_loops: %d\n", (int)((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec)), num_threads, num_barriers);
	return 0;
}

