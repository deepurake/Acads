#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main(int argc, char **argv)
{
	int num_threads = 3;
	int num_barriers = 3;
  // Serial code
	if(argc == 3)
	{
		num_threads = atoi(argv[1]);
		num_barriers = atoi(argv[2]);
	}
 struct timeval start, end;

  	printf("START, SenseReversal Barrier, num_threads: %d, num_barriers %d\n", num_threads,num_barriers);
	omp_set_num_threads(num_threads);

	// Globabl barrier variables
	int count = num_threads;
	int sense = 1;
#pragma omp parallel
  {
  	// local variables for barrier
  	int local_sense = 1;
  	int mycount = 0;
    // Now we're in the parallel section
    //int num_threads = omp_get_num_threads();
    int thread_num = omp_get_thread_num();
    int idx = 0;
    //printf("the Entered thread %d\n",thread_num);
	if(thread_num == 0)
	{
		gettimeofday(&start, NULL);
	}
    for (idx = 0; idx<num_barriers; idx++)
    {
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
			
    	//printf("After barrier %d thread %d\n",idx+1, thread_num);
    }
	if(thread_num == 0)
	{
		gettimeofday(&end, NULL);
	}
    
    // The code for the barrier
    //printf("Exiting the thread %d\n", thread_num);
  } 

  // Resume serial code
  //printf("Program completed\n");

    printf("EXIT,SenseReversal Barrier, time: %d, num_proc: %d, num_loops: %d\n", (int)((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec)), num_threads, num_barriers);
	return 0;
}

