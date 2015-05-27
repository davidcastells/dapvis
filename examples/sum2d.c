// take it from http://www.cse.iitd.ac.in/~subodh/courses/CSL730/pdfslides/3OpenMP.pdf

#include "mem_trace.h"

#define DIM_SIZE	40

double A[DIM_SIZE][DIM_SIZE];

double sum2d(double data[DIM_SIZE][DIM_SIZE]) ;

static void init_array()
{
    int i, j;
    int alpha = 12435;
    for (i = 0;
        i < DIM_SIZE;
        )
    {
        for (j = 0;
            j < DIM_SIZE;
            )
        {
            A[i][j] = ((double) i * j) / DIM_SIZE;
            //B[i][j] = ((double) i * j + 1) / DIM_SIZE;
            j++;
        }
        i++;
    }
}

int main(int argc, char **argv)
{
	init_array();
	double sum = sum2d(A);
	printf("SUM = %f", sum);
}


double sum2d(double data[DIM_SIZE][DIM_SIZE]) 
{
 int i,j;
 double sum = 0;
// #pragma omp parallel for
	mem_trace_enable(1);
	
	mem_trace_def_array2d("data", DIM_SIZE, DIM_SIZE);
	mem_trace_def_mem("data", data, sizeof(double), sizeof(data));

		mem_trace_def_array1d("sum", 1);
	mem_trace_def_mem("sum", &sum, sizeof(double), sizeof(sum));

	mem_trace_loop_start("loop1");
 for (i=0; i<DIM_SIZE; i++) 
 { 	 
 	 	mem_trace_iter_start("i", i);
	//mem_trace_loop_start("loop2");

 	 for (j=0; j<DIM_SIZE; j++) 
 	 {
 	 	  	 	//mem_trace_iter_start("j", j);

 	 	  	 	mem_trace_read("data", &data[i][j]);
 	 	  	 	mem_trace_read("sum", &sum);
 	 	  	 	mem_trace_write("sum", &sum);
 	 	 sum += data[i][j];
 	 	 
 	 	  	 	  	 	//mem_trace_iter_end("j", j);

 	 }
 	 
// 	 	mem_trace_loop_end("loop2");
 	 	mem_trace_iter_end("i", i);

 
 }
 	mem_trace_loop_end("loop1");

 return sum;
}