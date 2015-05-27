#include <string.h>
#include <math.h>
#include <stdio.h>

#define DIM_SIZE	10
//#define DIM_SIZE	2048

double X[DIM_SIZE][DIM_SIZE];
double A[DIM_SIZE][DIM_SIZE];
double B[DIM_SIZE][DIM_SIZE];
void init_array()
{
    int i, j;
    for (i = 0; i < DIM_SIZE;)
    {
        for (j = 0; j < DIM_SIZE;)
        {
            int r = rand();
            X[i][j] = ((double) i * (j + 1) + 1) / DIM_SIZE;
            A[i][j] = ((double) (i - 1) * (j + 4) + 2) / DIM_SIZE;
            B[i][j] = ((double) (i + 3) * (j + 7) + 3) / DIM_SIZE;
            j++;
        }
        i++;
    }
}
int main(int argc, char **argv)
{
    int t, i1, i2;
    int n = DIM_SIZE;
    int tsteps = 100;
    init_array();
    mem_trace_enable(1);
    mem_trace_def_array2d("X", DIM_SIZE, DIM_SIZE);
    mem_trace_def_mem("X", &X, sizeof(double), sizeof (X));
    mem_trace_def_array2d("B", DIM_SIZE, DIM_SIZE);
    mem_trace_def_mem("B", &B, sizeof(double), sizeof (B));
    for (t = 0;
        t < tsteps;
        t++)
    {
    	printf("t=%d\n", t);
        for (i1 = 0;
            i1 < n;
            i1++)
        {
        	mem_trace_flush();
        	    	printf("i1=%d\n", i1);

            mem_trace_iter_start("i1", i1);
            {
                for (i2 = 1;
                    i2 < n;
                    i2++)
                {
                    mem_trace_iter_start("i2", i2);
                    {
                        mem_trace_write("X", &X[i1][i2]);
                        mem_trace_read("X", &X[i1][i2]);
                        mem_trace_read("X", &X[i1][i2 - 1]);
                        mem_trace_read("B", &B[i1][i2 - 1]);
                        X[i1][i2] = X[i1][i2] - X[i1][i2 - 1] * A[i1][i2] / B[i1][i2 - 1];
                        mem_trace_write("B", &B[i1][i2]);
                        mem_trace_read("B", &B[i1][i2]);
                        mem_trace_read("B", &B[i1][i2 - 1]);
                        B[i1][i2] = B[i1][i2] - A[i1][i2] * A[i1][i2] / B[i1][i2 - 1];
                    }
                    mem_trace_iter_end("i2", i2);
                }
            }
            mem_trace_iter_end("i1", i1);
        }
        for (i1 = 0;
            i1 < n;
            i1++)
        {
            mem_trace_iter_start("i1", i1);
            {
                mem_trace_write("X", &X[i1][n - 1]);
                mem_trace_read("X", &X[i1][n - 1]);
                X[i1][n - 1] = X[i1][n - 1] / B[i1][n - 1];
            }
            mem_trace_iter_end("i1", i1);
        }
        for (i1 = 0;
            i1 < n;
            i1++)
        {
            mem_trace_iter_start("i1", i1);
            {
                for (i2 = 0;
                    i2 < n - 2;
                    i2++)
                {
                    mem_trace_iter_start("i2", i2);
                    {
                        mem_trace_write("X", &X[i1][n - i2 - 2]);
                        mem_trace_read("X", &X[i1][n - 2 - i2]);
                        mem_trace_read("X", &X[i1][n - 2 - i2 - 1]);
                        X[i1][n - i2 - 2] = (X[i1][n - 2 - i2] - X[i1][n - 2 - i2 - 1] * A[i1][n - i2 - 3]) / B[i1][n - 3 - i2];
                    }
                    mem_trace_iter_end("i2", i2);
                }
            }
            mem_trace_iter_end("i1", i1);
        }
        for (i1 = 1;
            i1 < n;
            i1++)
        {
            mem_trace_iter_start("i1", i1);
            {
                for (i2 = 0;
                    i2 < n;
                    i2++)
                {
                    mem_trace_iter_start("i2", i2);
                    {
                        mem_trace_write("X", &X[i1][i2]);
                        mem_trace_read("X", &X[i1][i2]);
                        mem_trace_read("X", &X[i1 - 1][i2]);
                        mem_trace_read("B", &B[i1 - 1][i2]);
                        X[i1][i2] = X[i1][i2] - X[i1 - 1][i2] * A[i1][i2] / B[i1 - 1][i2];
                        mem_trace_write("B", &B[i1][i2]);
                        mem_trace_read("B", &B[i1][i2]);
                        mem_trace_read("B", &B[i1 - 1][i2]);
                        B[i1][i2] = B[i1][i2] - A[i1][i2] * A[i1][i2] / B[i1 - 1][i2];
                    }
                    mem_trace_iter_end("i2", i2);
                }
            }
            mem_trace_iter_end("i1", i1);
        }
        for (i2 = 0;
            i2 < n;
            i2++)
        {
            mem_trace_iter_start("i2", i2);
            {
                mem_trace_write("X", &X[n - 1][i2]);
                mem_trace_read("X", &X[n - 1][i2]);
                X[n - 1][i2] = X[n - 1][i2] / B[n - 1][i2];
            }
            mem_trace_iter_end("i2", i2);
        }
        for (i1 = 0;
            i1 < n - 2;
            i1++)
        {
            for (i2 = 0;
                i2 < n;
                i2++)
            {
                mem_trace_iter_start("i2", i2);
                {
                    mem_trace_write("X", &X[n - 2 - i1][i2]);
                    mem_trace_read("X", &X[n - 2 - i1][i2]);
                    mem_trace_read("X", &X[n - i1 - 3][i2]);
                    X[n - 2 - i1][i2] = (X[n - 2 - i1][i2] - X[n - i1 - 3][i2] * A[n - 3 - i1][i2]) / B[n - 2 - i1][i2];
                }
                mem_trace_iter_end("i2", i2);
            }
        }
    }
    double total = 0;
    for (i1 = 0;
        i1 < n - 2;
        i1++)
    {
        for (i2 = 0;
            i2 < n;
            i2++)
        {
            total += X[i1][i2];
        }
    }
    printf("total=%f\n", total);
    return 0;
}
