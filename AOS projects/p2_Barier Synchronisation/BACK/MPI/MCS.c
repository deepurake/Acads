#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h> /* memset */
#include <unistd.h> /* close */
#include <assert.h>
#include <sys/time.h>

int main(int argc, char **argv)
{
	int num_threads = 3;
	int num_barriers = 3;
  // Serial code

	 struct timeval start, end;
	if(argc == 3)
	{
		num_threads = atoi(argv[1]);
		num_barriers = atoi(argv[2]);
	}
        printf("START,MCS Barrier, num_threads: %d, num_barriers %d\n", num_threads,num_barriers);


  omp_set_num_threads(num_threads);
  
  // initialize the shared barrier variables
  uint8_t arr_CN[num_threads][4];
  uint8_t dep_CN[num_threads][2];
	memset(&arr_CN[0][0],0,sizeof(arr_CN));
	memset(&dep_CN[0][0],0,sizeof(dep_CN));
	int i = 0;
	for(i = 0; i< num_threads; i++)
	{
		arr_CN[i][0] = 1-(i*4+1 < num_threads);
		arr_CN[i][1] = 1-(i*4+2 < num_threads);
		arr_CN[i][2] = 1-(i*4+3 < num_threads);
		arr_CN[i][3] = 1-(i*4+4 < num_threads);
		dep_CN[i][0] = 0;
		dep_CN[i][1] = 0;
	}
	
	//printf("for 2 : (%d %d %d %d)\n",arr_CN[2][0],arr_CN[2][1],arr_CN[2][2],arr_CN[2][3]);
	
	
#pragma omp parallel
  {
    // Now we're in the parallel section
    int num_threads = omp_get_num_threads();
    int t_num = omp_get_thread_num();
    int parent = (t_num +3)/4 -1;
    int dep_parent = (t_num+1)/2 -1;
    //printf("t_num %d, parent %d dep_pn %d\n",t_num,parent, dep_parent);
    //fflush(stdout);
    int idx = 0;
    
    
    // initialize the thread barrier variables
    // child 1, 2, 3 ,4
    uint8_t arr_HC[4] = {1,1,1,1};
    
    //printf("Entered the thread %d\n",t_num);
    //fflush(stdout);
	if(t_num == 0)
	{
		gettimeofday(&start, NULL);
	}
    for (idx = 0; idx<num_barriers; idx++)
    {
    	int temp = 0;
    	//arrival part

    	//wait for children
    	int t = 0;
    	while(!temp)
    	{
    		t++;
#pragma omp critical 
				{
					temp = (arr_CN[t_num][0]||(!arr_HC[0])) && (arr_CN[t_num][1]||(!arr_HC[1])) && (arr_CN[t_num][2]||(!arr_HC[2])) && (arr_CN[t_num][3]||(!arr_HC[3]));
					if(t%100000000 == 0)
    			{
    				t = 0;
    			}
				}
    	}
    	// inform parents
    	if(parent >= 0)
    	{
    		while(arr_CN[parent][(t_num+3)%4]);
    		arr_CN[parent][(t_num+3)%4] = 1;

    	}
    	
#pragma omp critical
			{
				arr_CN[t_num][0] = 1-(t_num*4+1 < num_threads);
				arr_CN[t_num][1] = 1-(t_num*4+2 < num_threads);
				arr_CN[t_num][2] = 1-(t_num*4+3 < num_threads);
				arr_CN[t_num][3] = 1-(t_num*4+4 < num_threads);
			}
			
			//departure tree
			
			// wait for parent
			if(dep_parent >= 0)
			{
				t = 0;
    		assert (((t_num+1)%2 == 0) || ((t_num+1)%2 == 1));
    		while(!dep_CN[dep_parent][(t_num+1)%2])
    		{
    			t++;
    			if(t%100000000 == 0)
    			{
    				t = 0;
    			}
    		}
    	}
#pragma omp critical
    	{
		  	if(dep_parent >=0)
		  	{
		  		dep_CN[dep_parent][(t_num+1)%2] = 0;
		  	}
				// wakeup childern
				dep_CN[t_num][0] = 1;
				dep_CN[t_num][1] = 1;
			}
    	
    }
    if(t_num == 0)
	{
		gettimeofday(&end, NULL);
	}  
    // The code for the barrier
    //printf("Exiting the thread %d\n", t_num);
  }

  // Resume serial code
  //printf("Program completed\n");
  //gettimeofday(&end, NULL);
	printf("EXIT,MCS Barrier, time: %d, num_proc: %d, num_loops: %d\n", (int)((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec)), num_threads, num_barriers);
  return 0;
}

