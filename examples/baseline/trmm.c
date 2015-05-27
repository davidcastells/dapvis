#include <string.h>
#include <math.h>

//#define DIM_SIZE	400
#define DIM_SIZE	40

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

    for (i = 1; i < n; i++)
    {
            for (j = 0; j < n; j++)
            {
                    for (k = 0; k < i; k++)
                    {
                        B[i][j] += alpha * A[i][k] * B[j][k];
                    }
            }
    }

    return 0;
}
