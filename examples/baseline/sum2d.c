// take it from http://www.cse.iitd.ac.in/~subodh/courses/CSL730/pdfslides/3OpenMP.pdf
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
	double sum = sum2d(A);
	printf("SUM = %f", sum);
}


double sum2d(double data[DIM_SIZE][DIM_SIZE]) 
{
 int i,j;
 double sum = 0;
// #pragma omp parallel for
 for (i=0; i<DIM_SIZE; i++) 
 {
 	 
 	 for (j=0; j<N; j++) 
 	 {
 	 	 sum += data[i][j];
 	 }
 }
 return sum;
}