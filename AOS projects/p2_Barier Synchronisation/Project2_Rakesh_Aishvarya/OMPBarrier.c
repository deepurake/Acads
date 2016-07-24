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

  printf("START,OpenMP Barrier, num_threads: %d, num_barriers %d\n", num_threads,num_barriers);

  // profiling code
  struct timeval start, end;

  // Initialize multiple threads
  omp_set_num_threads(num_threads);
#pragma omp parallel
  {
    // local variables
    int thread_num = omp_get_thread_num();
    int idx = 0;

    // profiling code
    if(thread_num == 0)
    {
      gettimeofday(&start, NULL);
    }
    for (idx = 0; idx<num_barriers; idx++)
    {
      // Call the barrier
#pragma omp barrier
      //printf("After barrier %d thread %d\n",idx+1, thread_num);
    }
    //printf("Exiting the thread %d\n", thread_num);

    // Profiling code
    if(thread_num == 0)
    {
      gettimeofday(&end, NULL);
    }
  } 

  // Resume serial code
  printf("EXIT,OpenMP Barrier, time: %d, num_proc: %d, num_loops: %d\n", (int)((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec)), num_threads, num_barriers);
  return 0;
}

