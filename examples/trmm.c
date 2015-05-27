#include <string.h>
#include <math.h>

//#define DIM_SIZE	400
#define DIM_SIZE	20

double alpha;
double A[DIM_SIZE][DIM_SIZE];
double B[DIM_SIZE][DIM_SIZE];
static void init_array()
{
    int i, j;
    alpha = 12435;
    for (i = 0;
        i < DIM_SIZE;
        )
    {
        for (j = 0;
            j < DIM_SIZE;
            )
        {
            A[i][j] = ((double) i * j) / DIM_SIZE;
            B[i][j] = ((double) i * j + 1) / DIM_SIZE;
            j++;
        }
        i++;
    }
}
int main(int argc, char **argv)
{
    int i, j, k;
    int n = DIM_SIZE;
    init_array();
        mem_trace_enable(1);

    mem_trace_def_array2d("B", DIM_SIZE, DIM_SIZE);
    mem_trace_def_mem("B", &B, sizeof(double), sizeof (B));
    mem_trace_def_array2d("A", DIM_SIZE, DIM_SIZE);
    mem_trace_def_mem("A", &A, sizeof(double), sizeof (A));
    mem_trace_loop_start("loop1");
    for (i = 1;
        i < n;
        i++)
    {
        mem_trace_iter_start("i", i);
        {
        	// #pragma parallel for
        	    mem_trace_loop_start("loop2");

            for (j = 0;
                j < n;
                j++)
            {
                mem_trace_iter_start("j", j);
                {
                    for (k = 0;
                        k < i;
                        k++)
                    {
                        mem_trace_read("A", &A[i][k]);
                        mem_trace_read("B", &B[j][k]);
                        mem_trace_write("B", &B[i][j]);
                        B[i][j] += alpha * A[i][k] * B[j][k];
                    }
                }
                mem_trace_iter_end("j", j);
            }
            
                mem_trace_loop_end("loop2");

        }
        mem_trace_iter_end("i", i);
    }

    mem_trace_loop_end("loop1");

    return 0;
}
