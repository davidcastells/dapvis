//#define PROBLEM_SIZE	4096
#define PROBLEM_SIZE	40

double A[PROBLEM_SIZE][PROBLEM_SIZE];
double x[PROBLEM_SIZE];
double y[PROBLEM_SIZE];
double tmp[PROBLEM_SIZE];
static void init_array()
{
    int i, j;
    for (i = 0;
        i < PROBLEM_SIZE;
        )
    {
        x[i] = i * 2;
        for (j = 0;
            j < PROBLEM_SIZE;
            )
        {
            A[i][j] = ((double) i * j) / (double)PROBLEM_SIZE;
            j++;
        }
        i++;
    }
}
int main(int argc, char **argv)
{
    int i, j;
    int nx = PROBLEM_SIZE;
    int ny = PROBLEM_SIZE;
    init_array();
    mem_trace_enable(1);

    mem_trace_def_array1d("y", PROBLEM_SIZE);
    mem_trace_def_mem("y", &y, sizeof(double), sizeof (y));
    mem_trace_def_array1d("tmp", PROBLEM_SIZE);
    mem_trace_def_mem("tmp", &tmp, sizeof(double), sizeof (tmp));
    mem_trace_def_array2d("A", PROBLEM_SIZE, PROBLEM_SIZE);
    mem_trace_def_mem("A", &A, sizeof(double), sizeof (A));
    for (i = 0;
        i < nx;
        i++)
    {
        //mem_trace_iter_start("i", i);
        {
            //mem_trace_write("y", &y[i]);
            (y[i] = 0);
        }
        //mem_trace_iter_end("i", i);
    }
    
    mem_trace_loop_start("loop1");
    
    for (i = 0;
        i < ny;
        i++)
    {
        mem_trace_iter_start("i", i);
        {
            mem_trace_write("tmp", &tmp[i]);
            (tmp[i] = 0);
            
            mem_trace_loop_start("loop2");
    
            for (j = 0;
                j < ny;
                j++)
            {
                mem_trace_iter_start("j", j);
                {
                    mem_trace_write("tmp", &tmp[i]);
                    mem_trace_read("tmp", &tmp[i]);
                    mem_trace_read("A", &A[i][j]);
                    (tmp[i] = tmp[i] + A[i][j] * x[j]);
                }
                mem_trace_iter_end("j", j);
            }
            
            mem_trace_loop_end("loop2");
    
            mem_trace_loop_start("loop3");
    
            for (j = 0;
                j < ny;
                j++)
            {
                mem_trace_iter_start("j", j);
                {
                    mem_trace_write("y", &y[j]);
                    mem_trace_read("y", &y[j]);
                    mem_trace_read("A", &A[i][j]);
                    mem_trace_read("tmp", &tmp[i]);
                    (y[j] = y[j] + A[i][j] * tmp[i]);
                }
                mem_trace_iter_end("j", j);
            }
            
            mem_trace_loop_start("loop3");
    
        }
        mem_trace_iter_end("i", i);
    }
    
    mem_trace_loop_end("loop1");
    
    double total = 0;
    for (int iter = 0;
        iter < ny;
        iter++)
    {
        total += y[iter];
    }
    printf("total=%f\n", total);
    return 0;
}
