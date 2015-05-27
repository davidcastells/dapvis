/*
Translated to C by Bonnie Toy 5/88

You MUST specify one of -DSP   or -DDP     to compile correctly.
You MUST specify one of -DROLL or -DUNROLL to compile correctly.
You MUST specify a timer option(see below) to compile correctly.

To compile double precision version for Sun-4:
   cc -DUNIX -DDP -DROLL -O4 clinpack.c

To compile single precision version for Sun-4:
   cc -DUNIX -DSP -DROLL -O4 -fsingle -fsingle2 clinpack.c

To obtain   rolled source BLAS, add -DROLL   to the command lines.
To obtain unrolled source BLAS, add -DUNROLL to the command lines.

PLEASE NOTE: You can also just 'uncomment' one of the options below.
*/

/* #define SP     */
/* #define DP     */
/* #define ROLL   */
/* #define UNROLL */

/***************************************************************/
/* Timer options. You MUST uncomment one of the options below  */
/* or compile, for example, with the '-DUNIX' option.          */
/***************************************************************/
/* #define Amiga       */
/* #define UNIX        */
/* #define UNIX_Old    */
/* #define VMS         */
/* #define BORLAND_C   */
/* #define MSC         */
/* #define MAC         */
/* #define IPSC        */
/* #define FORTRAN_SEC */
/* #define GTODay      */
/* #define CTimer      */
/* #define UXPM        */
/* #define MAC_TMgr    */
/* #define PARIX       */
/* #define POSIX       */
/* #define WIN32       */
/* #define POSIX1      */
/***********************/

#include <stdio.h>
#include <math.h>
#ifdef OMPC
#include <omp.h>
#ifdef __linux__
#include <unistd.h>
#endif
#include <stdlib.h>
#endif

#ifdef SP
#define REAL float
#define ZERO 0.0
#define ONE  1.0
#define PREC "Single "
#endif

#ifdef DP
#define REAL double
#define ZERO 0.0e0
#define ONE  1.0e0
#define PREC "Double "
#endif

#ifndef NTIMES
#define NTIMES 100
#endif

#ifndef ORDER
#define ORDER 100
#endif
#define ORDER2		(ORDER*2)
#define ORDER2P1	(ORDER*2+1)

#ifdef ROLL
#define ROLLING "Rolled "
#endif

#ifdef UNROLL
#define ROLLING "Unrolled "
#endif

static double st[8][6];

#ifdef OMPC
int	bf = 10;
#endif

#include "mem_trace.h"

REAL epslon (REAL x);
void matgen(REAL a[], int lda, int n, REAL b[], REAL *norma);
void dscal(int n, REAL da, REAL dx[], int incx);
void dgefa(REAL a[], int lda, int n, int ipvt[], int* info);
void daxpy(int n, REAL da, REAL dx[], int incx, REAL dy[], int incy);

int main (int argc, char* argv[])
{
   static REAL aa[ORDER2][ORDER2],a[ORDER2][ORDER2P1],b[ORDER2],x[ORDER2];
   REAL cray,ops,total,norma,normx;
   REAL resid,residn,eps;
   REAL kf;
   double t1,tm,tm2,dtime();
   static int ipvt[ORDER2],n,i,ntimes,info,lda,ldaa,kflops;

   lda = ORDER2P1;
   ldaa = ORDER2;
   cray = .056; 
   n = ORDER;

   mem_trace_enable(1);
   mem_trace_def_array1d("ipvt", ORDER2);
   mem_trace_def_mem("ipvt", ipvt, sizeof(int), sizeof(ipvt));
   mem_trace_def_array2d("a", ORDER2, ORDER2P1);
   mem_trace_def_mem("a", aa, sizeof(REAL), sizeof(a)); 
   mem_trace_enable(0);
#ifdef OMPC
   {
     int		c;
     extern char	*optarg;

     while ((c = getopt (argc, argv, "b:")) != EOF) {
       switch (c) {
       case 'b':
	 bf = atoi (optarg);
	 break;
       }
     }
   }

   if (omp_get_max_threads () != 1) {
     printf ("OpenMP(%d threads)\n", omp_get_max_threads ());
   } else {
     printf ("OpenMP(1 thread)\n");
   }
#endif
   printf(ROLLING); printf(PREC);
   printf("Precision Linpack\n\n");

       ops = (2.0e0*(n*n*n))/3.0 + 2.0*(n*n);

       matgen((REAL*)a,lda,n,b,&norma);
       t1 = dtime();
       dgefa(a,lda,n,ipvt,&info);
       st[0][0] = dtime() - t1;
       
       t1 = dtime();
       dgesl(a,lda,n,ipvt,b,0);
       st[1][0] = dtime() - t1;
       total = st[0][0] + st[1][0];

/*     compute a residual to verify results.  */ 

       for (i = 0; i < n; i++)
	  {
		x[i] = b[i];
	  }
       matgen((REAL*)a,lda,n,b,&norma);
       for (i = 0; i < n; i++) 
	  {
		b[i] = -b[i];
	  }
       dmxpy(n,b,n,lda,x,a);
       resid = 0.0;
       normx = 0.0;
       for (i = 0; i < n; i++)
	{
		resid = (resid > fabs((double)b[i])) 
	? resid : fabs((double)b[i]);
		normx = (normx > fabs((double)x[i])) 
	? normx : fabs((double)x[i]);
	}
       eps = epslon((REAL)ONE);
       residn = resid/( n*norma*normx*eps );
   
   printf("   norm. resid      resid           machep");
   printf("         x[0]-1        x[n-1]-1\n");
   printf("%8.1f      %16.8e%16.8e%16.8e%16.8e\n",
	 (double)residn, (double)resid, (double)eps, 
		(double)x[0]-1, (double)x[n-1]-1);

printf(" times are reported for matrices of order %5d\n",n);
printf("      dgefa      dgesl      total       kflops     unit");
printf("      ratio\n");

       st[2][0] = total;
       st[3][0] = ops/(1.0e3*total);
       st[4][0] = 2.0e3/st[3][0];
       st[5][0] = total/cray;

   printf(" times for array with leading dimension of%5d\n",lda);
   print_time(0);

       matgen(a,lda,n,b,&norma);
       t1 = dtime();
       dgefa(a,lda,n,ipvt,&info);
       st[0][1] = dtime() - t1;
       
       t1 = dtime();
       dgesl(a,lda,n,ipvt,b,0);
       st[1][1] = dtime() - t1;
       total = st[0][1] + st[1][1];
       
       st[2][1] = total;
       st[3][1] = ops/(1.0e3*total);
       st[4][1] = 2.0e3/st[3][1];
       st[5][1] = total/cray;

       matgen(a,lda,n,b,&norma);
       
       t1 = dtime();
       dgefa(a,lda,n,ipvt,&info);
       st[0][2] = dtime() - t1;
       
       t1 = dtime();
       dgesl(a,lda,n,ipvt,b,0);
       st[1][2] = dtime() - t1;
       
       total = st[0][2] + st[1][2];
       st[2][2] = total;
       st[3][2] = ops/(1.0e3*total);
       st[4][2] = 2.0e3/st[3][2];
       st[5][2] = total/cray;

       ntimes = NTIMES;
       tm2 = 0.0;
       t1 = dtime();

   for (i = 0; i < ntimes; i++) {
		tm = dtime();
       matgen(a,lda,n,b,&norma);
       tm2 = tm2 + dtime() - tm;
       dgefa(a,lda,n,ipvt,&info);
       }

       st[0][3] = (dtime() - t1 - tm2)/ntimes;
       t1 = dtime();

       for (i = 0; i < ntimes; i++) 
       {
		dgesl(a,lda,n,ipvt,b,0);
       }

       st[1][3] = (dtime() - t1)/ntimes;
       total = st[0][3] + st[1][3];
       st[2][3] = total;
       st[3][3] = ops/(1.0e3*total);
       st[4][3] = 2.0e3/st[3][3];
       st[5][3] = total/cray;

   print_time(1);
   print_time(2);
   print_time(3);

       matgen(aa,ldaa,n,b,&norma);
       t1 = dtime();
       dgefa(aa,ldaa,n,ipvt,&info);
       st[0][4] = dtime() - t1;
       
       t1 = dtime();
       dgesl(aa,ldaa,n,ipvt,b,0);
       st[1][4] = dtime() - t1;

       total = st[0][4] + st[1][4];
       st[2][4] = total;
       st[3][4] = ops/(1.0e3*total);
       st[4][4] = 2.0e3/st[3][4];
       st[5][4] = total/cray;

       matgen(aa,ldaa,n,b,&norma);
       t1 = dtime();
       dgefa(aa,ldaa,n,ipvt,&info);
       st[0][5] = dtime() - t1;

       t1 = dtime();
       dgesl(aa,ldaa,n,ipvt,b,0);
       st[1][5] = dtime() - t1;

       total = st[0][5] + st[1][5];
       st[2][5] = total;
       st[3][5] = ops/(1.0e3*total);
       st[4][5] = 2.0e3/st[3][5];
       st[5][5] = total/cray;

   matgen(aa,ldaa,n,b,&norma);
   t1 = dtime();
   dgefa(aa,ldaa,n,ipvt,&info);
   st[0][6] = dtime() - t1;

   t1 = dtime();
   dgesl(aa,ldaa,n,ipvt,b,0);
   st[1][6] = dtime() - t1;

   total = st[0][6] + st[1][6];
   st[2][6] = total;
   st[3][6] = ops/(1.0e3*total);
   st[4][6] = 2.0e3/st[3][6];
   st[5][6] = total/cray;

	   ntimes = NTIMES;
	   tm2 = 0;
	   t1 = dtime();
   
	   mem_trace_enable(1);
	   for (i = 0; i < ntimes; i++)
	   {
	   	   tm = dtime();
	   	   matgen(aa,ldaa,n,b,&norma);
	   	   tm2 = tm2 + dtime() - tm;
	   	   dgefa(aa,ldaa,n,ipvt,&info);
	   	   	   
	   	   mem_trace_enable(0);

       }

   st[0][7] = (dtime() - t1 - tm2)/ntimes;
   
   t1 = dtime();
   for (i = 0; i < ntimes; i++) {
       dgesl(aa,ldaa,n,ipvt,b,0);
       }

   st[1][7] = (dtime() - t1)/ntimes;
   total = st[0][7] + st[1][7];
   st[2][7] = total;
   st[3][7] = ops/(1.0e3*total);
   st[4][7] = 2.0e3/st[3][7];
   st[5][7] = total/cray;

   /* the following code sequence implements the semantics of
       the Fortran intrinsics "nint(min(st[3][3],st[3][7]))"   */
/*
   kf = (st[3][3] < st[3][7]) ? st[3][3] : st[3][7];
   kf = (kf > ZERO) ? (kf + .5) : (kf - .5);
   if (fabs((double)kf) < ONE) 
       kflops = 0;
   else {
       kflops = floor(fabs((double)kf));
       if (kf < ZERO) kflops = -kflops;
   }
*/
   if ( st[3][3] < ZERO ) st[3][3] = ZERO;
   if ( st[3][7] < ZERO ) st[3][7] = ZERO;
   kf = st[3][3];
   if ( st[3][7] < st[3][3] ) kf = st[3][7];
   kflops = (int)(kf + 0.5);

   printf(" times for array with leading dimension of%5d\n",ldaa);
   print_time(4);
   print_time(5);
   print_time(6);
   print_time(7);
   printf(ROLLING); printf(PREC);
   printf(" Precision %5d Kflops ; %d Reps \n",kflops,NTIMES);
}
     
/*----------------------*/ 
print_time (int row)
{
printf("%11.2f%11.2f%11.2f%11.0f%11.2f%11.2f\n",
	(double)st[0][row], (double)st[1][row], (double)st[2][row], 
	(double)st[3][row], (double)st[4][row], (double)st[5][row]);
}
       
/*----------------------*/ 


/* We would like to declare a[][lda], but c does not allow it.  In this
function, references to a[i][j] are written a[lda*i+j].  */
void matgen(REAL a[], int lda, int n, REAL b[], REAL *norma)
{
   int init, i, j;

   init = 1325;
   *norma = 0.0;
   for (j = 0; j < n; j++) {
       for (i = 0; i < n; i++) {
	init = 3125*init % 65536;
	a[lda*j+i] = (init - 32768.0)/16384.0;
	*norma = (a[lda*j+i] > *norma) ? a[lda*j+i] : *norma;
       }
   }
   for (i = 0; i < n; i++) {
	 b[i] = 0.0;
   }
   for (j = 0; j < n; j++) {
       for (i = 0; i < n; i++) {
	b[i] = b[i] + a[lda*j+i];
       }
   }
}

/*----------------------*/ 


/* We would like to declare a[][lda], but c does not allow it.  In this
function, references to a[i][j] are written a[lda*i+j].  
*/

/*
     dgefa factors a double precision matrix by gaussian elimination.

     dgefa is usually called by dgeco, but it can be called
     directly with a saving in time if  rcond  is not needed.
     (time for dgeco) = (1 + 9/n)*(time for dgefa) .

     on entry

       a       REAL precision[n][lda]
	       the matrix to be factored.

       lda     integer
	       the leading dimension of the array  a .

       n       integer
	       the order of the matrix  a .

     on return

       a       an upper triangular matrix and the multipliers
	       which were used to obtain it.
	       the factorization can be written  a = l*u  where
	       l  is a product of permutation and unit lower
	       triangular matrices and  u  is upper triangular.

       ipvt    integer[n]
	       an integer vector of pivot indices.

       info    integer
	       = 0  normal value.
	       = k  if  u[k][k] .eq. 0.0 .  this is not an error
		    condition for this subroutine, but it does
		    indicate that dgesl or dgedi will divide by zero
		    if called.  use  rcond  in dgeco for a reliable
		    indication of singularity.

     linpack. this version dated 08/14/78 .
     cleve moler, university of new mexico, argonne national lab.

     functions

     blas daxpy,dscal,idamax
*/

void dgefa(REAL a[], int lda, int n, int ipvt[], int* info)
{
	/*     internal variables   */

	REAL t;
	int j,k,l,nm1;


	/* gaussian elimination with partial pivoting   */

   *info = 0;
   nm1 = n - 1;
   if (nm1 >=  0)
   {
   	   mem_trace_loop_start("loop1");
   	   
   	   for (k = 0; k < nm1; k++)
   	   {
   	   	   int kp1;
   	   	   mem_trace_iter_start("k", k);
   	   	   
   	   	   kp1 = k + 1;

	       /* find l = pivot index   */
	       mem_trace_set_alias("dx", "a");
	       l = idamax(n-k,&a[lda*k+k],1) + k;
	       ipvt[k] = l;
	       mem_trace_write("ipvt", k);
	       
	       /* zero pivot implies this column already 
	       triangularized */

	       mem_trace_read("a", &a[lda*k+l]);
	       if (a[lda*k+l] != ZERO) 
	       {

	       	   /* interchange if necessary */

	       	   if (l != k)
	       	   {
	       	   	   mem_trace_read("a", &a[lda*k+l]);
	       	   	   t = a[lda*k+l];
	       	   	   mem_trace_read("a", &a[lda*k+k]);
	       	   	   a[lda*k+l] = a[lda*k+k];
	       	   	   a[lda*k+k] = t; 
	       	   	   mem_trace_write("a", &a[lda*k+l]);
	       	   	   mem_trace_write("a", &a[lda*k+k]);
	       	   }

	       	   /* compute multipliers */
	       	   mem_trace_read("a", &a[lda*k+k]);
	       	   t = -ONE/a[lda*k+k];
	       	   
	       	   mem_trace_set_alias("dx", "a");
	       	   dscal(n-(k+1),t,&a[lda*k+k+1],1);

	       	   /* row elimination with column indexing */

	       	   mem_trace_loop_start("loop2");
   	   
	       	   // CANDIDATE
	       	   for (j = kp1; j < n; j++) 
	       	   {
	       	   	   mem_trace_iter_start("j", j);
	       	   	   
	       	   	   mem_trace_read("a", &a[lda*j+l]);
	       	   	   t = a[lda*j+l];
	       	   	   if (l != k)
	       	   	   {
	       	   	   	   mem_trace_read("a", &a[lda*j+k]);
	       	   	   	   a[lda*j+l] = a[lda*j+k];
	       	   	   	   a[lda*j+k] = t;
	       	   	   	   mem_trace_write("a", &a[lda*j+l]);
	       	   	   	   mem_trace_write("a", &a[lda*j+k]);
	       	   	   	   
	       	   	   }
	       	   	   mem_trace_set_alias("dx", "a");
	       	   	   mem_trace_set_alias("dy", "a");
	       	   	   daxpy(n-(k+1),t,&a[lda*k+k+1],1, &a[lda*j+k+1],1);
	       	   	   

	       	   	   mem_trace_iter_end("j", j);

	       	   } 
	       	   
	       	   mem_trace_loop_end("loop2");
   	   
	       }
	       else 
	       { 
	       	   *info = k;
	       }
	       
	       mem_trace_iter_end("k", k);
   	   	   
       } 
       
       mem_trace_loop_end("loop1");
   	   

   }
   ipvt[n-1] = n-1;
   if (a[lda*(n-1)+(n-1)] == ZERO) *info = n-1;
}


/*----------------------*/ 

dgesl(a,lda,n,ipvt,b,job)
int lda,n,ipvt[],job;
REAL a[],b[];

/* We would like to declare a[][lda], but c does not allow it.  In this
function, references to a[i][j] are written a[lda*i+j].  */

/*
     dgesl solves the double precision system
     a * x = b  or  trans(a) * x = b
     using the factors computed by dgeco or dgefa.

     on entry

       a       double precision[n][lda]
	       the output from dgeco or dgefa.

       lda     integer
	       the leading dimension of the array  a .

       n       integer
	       the order of the matrix  a .

       ipvt    integer[n]
	       the pivot vector from dgeco or dgefa.

       b       double precision[n]
	       the right hand side vector.

       job     integer
	       = 0         to solve  a*x = b ,
	       = nonzero   to solve  trans(a)*x = b  where
			   trans(a)  is the transpose.

    on return

       b       the solution vector  x .

     error condition

       a division by zero will occur if the input factor contains a
       zero on the diagonal.  technically this indicates singularity
       but it is often caused by improper arguments or improper
       setting of lda .  it will not occur if the subroutines are
       called correctly and if dgeco has set rcond .gt. 0.0
       or dgefa has set info .eq. 0 .

     to compute  inverse(a) * c  where  c  is a matrix
     with  p  columns
	  dgeco(a,lda,n,ipvt,rcond,z)
	  if (!rcond is too small){
	       for (j=0,j<p,j++)
		   dgesl(a,lda,n,ipvt,c[j][0],0);
       }

     linpack. this version dated 08/14/78 .
     cleve moler, university of new mexico, argonne national lab.

     functions

     blas daxpy,ddot
*/


{
/*     internal variables   */

   REAL ddot(),t;
   int k,kb,l,nm1;

   nm1 = n - 1;
   if (job == 0) {

       /* job = 0 , solve  a * x = b
	first solve  l*y = b       */

       if (nm1 >= 1) {
	for (k = 0; k < nm1; k++) {
	   l = ipvt[k];
	   t = b[l];
	   if (l != k){ 
		b[l] = b[k];
		b[k] = t;
	   }   
	   daxpy(n-(k+1),t,&a[lda*k+k+1],1,&b[k+1],1);
	}
       } 

       /* now solve  u*x = y */

       for (kb = 0; kb < n; kb++) {
	 k = n - (kb + 1);
	 b[k] = b[k]/a[lda*k+k];
	 t = -b[k];
	 daxpy(k,t,&a[lda*k+0],1,&b[0],1);
       }
   }
   else { 

       /* job = nonzero, solve  trans(a) * x = b
	first solve  trans(u)*y = b          */

       for (k = 0; k < n; k++) {
	t = ddot(k,&a[lda*k+0],1,&b[0],1);
	b[k] = (b[k] - t)/a[lda*k+k];
       }

       /* now solve trans(l)*x = y   */

       if (nm1 >= 1) {
	for (kb = 1; kb < nm1; kb++) {
	   k = n - (kb+1);
	   b[k] = b[k] + ddot(n-(k+1),&a[lda*k+k+1],1,&b[k+1],1);
	   l = ipvt[k];
	   if (l != k) {
		t = b[l];
		b[l] = b[k];
		b[k] = t;
	   }
	}
       }
   }
}

/*----------------------*/ 

/*
     constant times a vector plus a vector.
     jack dongarra, linpack, 3/11/78.
*/
void daxpy(int n, REAL da, REAL dx[], int incx, REAL dy[], int incy)
{
   int i,ix,iy,m,mp1;

   if(n <= 0) return;
   if (da == ZERO) return;

   if(incx != 1 || incy != 1) 
   {

       /* code for unequal increments or equal increments
	not equal to 1                */

       ix = 1;
       iy = 1;
       if(incx < 0) ix = (-n+1)*incx + 1;
       if(incy < 0) iy = (-n+1)*incy + 1;
       for (i = 0;i < n; i++) 
       {
       	   mem_trace_read("dy", &dy[iy]);
       	   mem_trace_read("dx", &dx[ix]);
       	   mem_trace_write("dy", &dy[iy]);
       	   dy[iy] = dy[iy] + da*dx[ix];
	ix = ix + incx;
	iy = iy + incy;
       }
	   return;
   }

   /* code for both increments equal to 1 */

   for (i = 0;i < n; i++) 
   {
   	    mem_trace_read("dy", &dy[i]);
       	   mem_trace_read("dx", &dx[i]);
       	   mem_trace_write("dy", &dy[i]);
       dy[i] = dy[i] + da*dx[i];
   }

}
   
/*----------------------*/ 

REAL ddot(n,dx,incx,dy,incy)
/*
     forms the dot product of two vectors.
     jack dongarra, linpack, 3/11/78.
*/
REAL dx[],dy[];

int incx,incy,n;
{
   REAL dtemp;
   int i,ix,iy,m,mp1;

   dtemp = ZERO;

   if(n <= 0) return(ZERO);

   if(incx != 1 || incy != 1) {

       /* code for unequal increments or equal increments
	not equal to 1               */

       ix = 0;
       iy = 0;
       if (incx < 0) ix = (-n+1)*incx;
       if (incy < 0) iy = (-n+1)*incy;
       for (i = 0;i < n; i++) {
	dtemp = dtemp + dx[ix]*dy[iy];
	ix = ix + incx;
	iy = iy + incy;
       }
       return(dtemp);
   }

   /* code for both increments equal to 1 */

#ifdef ROLL
   for (i=0;i < n; i++)
       dtemp = dtemp + dx[i]*dy[i];
   return(dtemp);
#endif
#ifdef UNROLL

   m = n % 5;
   if (m != 0) {
       for (i = 0; i < m; i++)
	dtemp = dtemp + dx[i]*dy[i];
       if (n < 5) return(dtemp);
   }
   for (i = m; i < n; i = i + 5) {
       dtemp = dtemp + dx[i]*dy[i] +
       dx[i+1]*dy[i+1] + dx[i+2]*dy[i+2] +
       dx[i+3]*dy[i+3] + dx[i+4]*dy[i+4];
   }
   return(dtemp);
#endif
}

/*----------------------*/ 

/*     scales a vector by a constant.
       jack dongarra, linpack, 3/11/78.
*/
void dscal(int n, REAL da, REAL dx[], int incx)
{
   int i,m,mp1,nincx;

   if(n <= 0)return;
   if(incx != 1) 
   {

       /* code for increment not equal to 1 */

       nincx = n*incx;
       for (i = 0; i < nincx; i = i + incx)
       {
//       	   mem_trace_read("dx", &dx[i]);
//       	   mem_trace_write("dx", &dx[i]);
       	   dx[i] = da*dx[i];
       }
       return;
   }

   /* code for increment equal to 1 */

   for (i = 0; i < n; i++)
   {

//   	   mem_trace_read("dx", &dx[i]);
//   	   mem_trace_write("dx", &dx[i]);

       dx[i] = da*dx[i];
       
   }

}

/*----------------------*/ 

/*
     finds the index of element having max. absolute value.
     jack dongarra, linpack, 3/11/78.
*/
int idamax(int n, REAL dx[], int incx)
{
   REAL dmax;
   int i, ix, itemp;

   if( n < 1 ) return(-1);
   if(n ==1 ) return(0);
   if(incx != 1) 
   {

       /* code for increment not equal to 1 */

       ix = 1;
//       mem_trace_read("dx", &dx[0]);
       dmax = fabs((double)dx[0]);
       ix = ix + incx;
       for (i = 1; i < n; i++) 
       {
       	  // mem_trace_read("dx", &dx[ix]);
       	   if(fabs((double)dx[ix]) > dmax)
       	   {
       	   	   itemp = i;
       	   	   dmax = fabs((double)dx[ix]);
       	   }
       	   ix = ix + incx;
       }
   }
   else 
   {

       /* code for increment equal to 1 */

       itemp = 0;
     //  mem_trace_read("dx", &dx[0]);
       dmax = fabs((double)dx[0]);
       for (i = 1; i < n; i++)
       {
   //    	   mem_trace_read("dx", &dx[i]);
       	   if(fabs((double)dx[i]) > dmax) 
       	   {
       	   	   itemp = i;
       	   	   dmax = fabs((double)dx[i]);
       	   }
       }
   }
   return (itemp);
}

/*----------------------*/ 
REAL epslon (REAL x)
/*
     estimate unit roundoff in quantities of size x.
*/

{
   REAL a,b,c,eps;
/*
     this program should function properly on all systems
     satisfying the following two assumptions,
       1.  the base used in representing dfloating point
	   numbers is not a power of three.
       2.  the quantity  a  in statement 10 is represented to 
	   the accuracy used in dfloating point variables
	   that are stored in memory.
     the statement number 10 and the go to 10 are intended to
     force optimizing compilers to generate code satisfying 
     assumption 2.
     under these assumptions, it should be true that,
	   a  is not exactly equal to four-thirds,
	   b  has a zero for its last bit or digit,
	   c  is not exactly equal to one,
	   eps  measures the separation of 1.0 from
		the next larger dfloating point number.
     the developers of eispack would appreciate being informed
     about any systems where these assumptions do not hold.

     *****************************************************************
     this routine is one of the auxiliary routines used by eispack iii
     to avoid machine dependencies.
     *****************************************************************

     this version dated 4/6/83.
*/

   a = 4.0e0/3.0e0;
   eps = ZERO;
   while (eps == ZERO) {
       b = a - ONE;
       c = b + b + b;
       eps = fabs((double)(c-ONE));
   }
   return(eps*fabs((double)x));
}
 
/*----------------------*/ 
dmxpy (n1, y, n2, ldm, x, m)
REAL y[], x[], m[];
int n1, n2, ldm;

/* We would like to declare m[][ldm], but c does not allow it.  In this
function, references to m[i][j] are written m[ldm*i+j].  */

/*
   purpose:
     multiply matrix m times vector x and add the result to vector y.

   parameters:

     n1 integer, number of elements in vector y, and number of rows in
	matrix m

     y double [n1], vector of length n1 to which is added 
	the product m*x

     n2 integer, number of elements in vector x, and number of columns
	in matrix m

     ldm integer, leading dimension of array m

     x double [n2], vector of length n2

     m double [ldm][n2], matrix of n1 rows and n2 columns

 ----------------------------------------------------------------------
*/
{
   int j,i,jmin;
   /* cleanup odd vector */

   j = n2 % 2;
   if (j >= 1) {
       j = j - 1;
       for (i = 0; i < n1; i++) 
		 y[i] = (y[i]) + x[j]*m[ldm*j+i];
   } 

   /* cleanup odd group of two vectors */

   j = n2 % 4;
   if (j >= 2) {
       j = j - 1;
       for (i = 0; i < n1; i++)
		 y[i] = ( (y[i])
			   + x[j-1]*m[ldm*(j-1)+i]) + x[j]*m[ldm*j+i];
   } 

   /* cleanup odd group of four vectors */

   j = n2 % 8;
   if (j >= 4) {
       j = j - 1;
       for (i = 0; i < n1; i++)
	y[i] = ((( (y[i])
	       + x[j-3]*m[ldm*(j-3)+i]) 
	       + x[j-2]*m[ldm*(j-2)+i])
	       + x[j-1]*m[ldm*(j-1)+i]) + x[j]*m[ldm*j+i];
   } 

   /* cleanup odd group of eight vectors */

   j = n2 % 16;
   if (j >= 8) {
       j = j - 1;
       for (i = 0; i < n1; i++)
	y[i] = ((((((( (y[i])
	       + x[j-7]*m[ldm*(j-7)+i]) + x[j-6]*m[ldm*(j-6)+i])
		 + x[j-5]*m[ldm*(j-5)+i]) + x[j-4]*m[ldm*(j-4)+i])
	       + x[j-3]*m[ldm*(j-3)+i]) + x[j-2]*m[ldm*(j-2)+i])
	       + x[j-1]*m[ldm*(j-1)+i]) + x[j]  *m[ldm*j+i];
   } 
   
   /* main loop - groups of sixteen vectors */

   jmin = (n2%16)+16;
   for (j = jmin-1; j < n2; j = j + 16) {
       for (i = 0; i < n1; i++) 
	y[i] = ((((((((((((((( (y[i])
		  + x[j-15]*m[ldm*(j-15)+i]) 
	   + x[j-14]*m[ldm*(j-14)+i])
		+ x[j-13]*m[ldm*(j-13)+i]) 
	   + x[j-12]*m[ldm*(j-12)+i])
		+ x[j-11]*m[ldm*(j-11)+i]) 
	   + x[j-10]*m[ldm*(j-10)+i])
		+ x[j- 9]*m[ldm*(j- 9)+i]) 
	   + x[j- 8]*m[ldm*(j- 8)+i])
		+ x[j- 7]*m[ldm*(j- 7)+i]) 
	   + x[j- 6]*m[ldm*(j- 6)+i])
		+ x[j- 5]*m[ldm*(j- 5)+i]) 
	   + x[j- 4]*m[ldm*(j- 4)+i])
		+ x[j- 3]*m[ldm*(j- 3)+i]) 
	   + x[j- 2]*m[ldm*(j- 2)+i])
		+ x[j- 1]*m[ldm*(j- 1)+i]) 
	   + x[j]   *m[ldm*j+i];
   }
}

/*****************************************************/
/* Various timer routines.                           */
/* Al Aburto, aburto@nosc.mil, 18 Feb 1997           */
/*                                                   */
/* t = dtime() outputs the current time in seconds.  */
/* Use CAUTION as some of these routines will mess   */
/* up when timing across the hour mark!!!            */
/*                                                   */
/* For timing I use the 'user' time whenever         */
/* possible. Using 'user+sys' time is a separate     */
/* issue.                                            */
/*                                                   */
/* Example Usage:                                    */
/* [timer options added here]                        */
/* main()                                            */
/* {                                                 */
/* double starttime,benchtime,dtime();               */
/*                                                   */
/* starttime = dtime();                              */ 
/* [routine to time]                                 */
/* benchtime = dtime() - starttime;                  */
/* }                                                 */
/*                                                   */
/* [timer code below added here]                     */
/*****************************************************/

/*********************************/
/* Timer code.                   */
/*********************************/
/*******************/
/*  Amiga dtime()  */
/*******************/
#ifdef Amiga
#include <ctype.h>
#define HZ 50

double dtime()
{
 double q;

 struct tt
       {
	long  days;
	long  minutes;
	long  ticks;
       } tt;

 DateStamp(&tt);

 q = ((double)(tt.ticks + (tt.minutes * 60L * 50L))) / (double)HZ;

 return q;
}
#endif

/*****************************************************/
/*  UNIX dtime(). This is the preferred UNIX timer.  */
/*  Provided by: Markku Kolkka, mk59200@cc.tut.fi    */
/*  HP-UX Addition by: Bo Thide', bt@irfu.se         */
/*****************************************************/
#ifdef UNIX
#include <sys/time.h>
#include <sys/resource.h>

#ifdef hpux
#include <sys/syscall.h>
#define getrusage(a,b) syscall(SYS_getrusage,a,b)
#endif

struct rusage rusage;

double dtime()
{
 double q;

 getrusage(RUSAGE_SELF,&rusage);

 q = (double)(rusage.ru_utime.tv_sec);
 q = q + (double)(rusage.ru_utime.tv_usec) * 1.0e-06;
	
 return q;
}
#endif

/***************************************************/
/*  UNIX_Old dtime(). This is the old UNIX timer.  */
/*  Make sure HZ is properly defined in param.h !! */
/***************************************************/
#ifdef UNIX_Old
#include <sys/types.h>
#include <sys/times.h>
#include <sys/param.h>

#ifndef HZ
#define HZ 60
#endif

struct tms tms;

double dtime()
{
 double q;

 times(&tms);

 q = (double)(tms.tms_utime) / (double)HZ;
	
 return q;
}
#endif

/*********************************************************/
/*  VMS dtime() for VMS systems.                         */
/*  Provided by: RAMO@uvphys.phys.UVic.CA                */
/*  Some people have run into problems with this timer.  */
/*********************************************************/
#ifdef VMS
#include time

#ifndef HZ
#define HZ 100
#endif

struct tbuffer_t
      {
       int proc_user_time;
       int proc_system_time;
       int child_user_time;
       int child_system_time;
      };

struct tbuffer_t tms;

double dtime()
{
 double q;

 times(&tms);

 q = (double)(tms.proc_user_time) / (double)HZ;
	
 return q;
}
#endif

/******************************/
/*  BORLAND C dtime() for DOS */
/******************************/
#ifdef BORLAND_C
#include <ctype.h>
#include <dos.h>
#include <time.h>

#define HZ 100
struct time tnow;

double dtime()
{
 double q;

 gettime(&tnow);

 q = 60.0 * (double)(tnow.ti_min);
 q = q + (double)(tnow.ti_sec);
 q = q + (double)(tnow.ti_hund)/(double)HZ;
	
 return q;
}
#endif

/**************************************/
/*  Microsoft C (MSC) dtime() for DOS */
/**************************************/
#ifdef MSC
#include <time.h>
#include <ctype.h>

#define HZ CLOCKS_PER_SEC
clock_t tnow;

double dtime()
{
 double q;

 tnow = clock();

 q = (double)tnow / (double)HZ;
	
 return q;
}
#endif

/*************************************/
/*  Macintosh (MAC) Think C dtime()  */
/*************************************/
#ifdef MAC
#include <time.h>

#define HZ 60

double dtime()
{
 double q;

 q = (double)clock() / (double)HZ;
	
 return q;
}
#endif

/************************************************************/
/*  iPSC/860 (IPSC) dtime() for i860.                       */
/*  Provided by: Dan Yergeau, yergeau@gloworm.Stanford.EDU  */
/************************************************************/
#ifdef IPSC
extern double dclock();

double dtime()
{
 double q;

 q = dclock();
	
 return q;
}
#endif

/**************************************************/
/*  FORTRAN dtime() for Cray type systems.        */
/*  This is the preferred timer for Cray systems. */
/**************************************************/
#ifdef FORTRAN_SEC

fortran double second();

double dtime()
{
 double q;

 second(&q);
	
 return q;
}
#endif

/***********************************************************/
/*  UNICOS C dtime() for Cray UNICOS systems.  Don't use   */
/*  unless absolutely necessary as returned time includes  */
/*  'user+system' time.  Provided by: R. Mike Dority,      */
/*  dority@craysea.cray.com                                */
/***********************************************************/
#ifdef CTimer
#include <time.h>

double dtime()
{
 double    q;
 clock_t   clock(void);

 q = (double)clock() / (double)CLOCKS_PER_SEC;

 return q;
}
#endif

/********************************************/
/* Another UNIX timer using gettimeofday(). */
/* However, getrusage() is preferred.       */
/********************************************/
#ifdef GTODay
#include <sys/time.h>

struct timeval tnow;

double dtime()
{
 double q;

 gettimeofday(&tnow,NULL);
 q = (double)tnow.tv_sec + (double)tnow.tv_usec * 1.0e-6;

 return q;
}
#endif

/*****************************************************/
/*  Fujitsu UXP/M timer.                             */
/*  Provided by: Mathew Lim, ANUSF, M.Lim@anu.edu.au */
/*****************************************************/
#ifdef UXPM
#include <sys/types.h>
#include <sys/timesu.h>
struct tmsu rusage;

double dtime()
{
 double q;

 timesu(&rusage);

 q = (double)(rusage.tms_utime) * 1.0e-06;
	
 return q;
}
#endif

/**********************************************/
/*    Macintosh (MAC_TMgr) Think C dtime()    */
/*   requires Think C Language Extensions or  */
/*    #include <MacHeaders> in the prefix     */
/*  provided by Francis H Schiffer 3rd (fhs)  */
/*         skipschiffer@genie.geis.com        */
/**********************************************/
#ifdef MAC_TMgr
#include <Timer.h>
#include <stdlib.h>

static TMTask   mgrTimer;
static Boolean  mgrInited = false;
static double   mgrClock;

#define RMV_TIMER RmvTime( (QElemPtr)&mgrTimer )
#define MAX_TIME  1800000000L
/* MAX_TIME limits time between calls to */
/* dtime( ) to no more than 30 minutes   */
/* this limitation could be removed by   */
/* creating a completion routine to sum  */
/* 30 minute segments (fhs 1994 feb 9)   */

static void Remove_timer( )
{
 RMV_TIMER;
 mgrInited = false;
}

double  dtime( )
{
 if( mgrInited ) {
   RMV_TIMER;
   mgrClock += (MAX_TIME + mgrTimer.tmCount)*1.0e-6;
 } else {
   if( _atexit( &Remove_timer ) == 0 ) mgrInited = true;
   mgrClock = 0.0;
 }
 
 if ( mgrInited )
   {
    mgrTimer.tmAddr = NULL;
    mgrTimer.tmCount = 0;
    mgrTimer.tmWakeUp = 0;
    mgrTimer.tmReserved = 0;
    InsTime( (QElemPtr)&mgrTimer );
    PrimeTime( (QElemPtr)&mgrTimer, -MAX_TIME );
   }
 return( mgrClock );
}
#endif

/***********************************************************/
/*  Parsytec GCel timer.                                   */
/*  Provided by: Georg Wambach, gw@informatik.uni-koeln.de */
/***********************************************************/
#ifdef PARIX
#include <sys/time.h>

double dtime()
{
 double q;

 q = (double) (TimeNowHigh()) / (double) CLK_TCK_HIGH;

 return q;
}
#endif

/************************************************/
/*  Sun Solaris POSIX dtime() routine           */
/*  Provided by: Case Larsen, CTLarsen.lbl.gov  */
/************************************************/
#ifdef POSIX
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/rusage.h>

#ifdef __hpux
#include <sys/syscall.h>
#endif

struct rusage rusage;

double dtime()
{
 double q;

 getrusage(RUSAGE_SELF,&rusage);

 q = (double)(rusage.ru_utime.tv_sec);
 q = q + (double)(rusage.ru_utime.tv_nsec) * 1.0e-09;
	
 return q;
}
#endif


/****************************************************/
/*  Windows NT (32 bit) dtime() routine             */
/*  Provided by: Piers Haken, piersh@microsoft.com  */
/****************************************************/
#ifdef WIN32
#include <windows.h>

double dtime(void)
{
 double q;

 q = (double)GetTickCount() * 1.0e-03;
	
 return q;
}
#endif

/*****************************************************/
/* Time according to POSIX.1  -  <J.Pelan@qub.ac.uk> */
/* Ref: "POSIX Programmer's Guide"  O'Reilly & Assoc.*/
/*****************************************************/
#ifdef POSIX1
#define _POSIX_SOURCE 1
#include <unistd.h>
#include <limits.h>
#include <sys/times.h>

struct tms tms;

double dtime()
{
 double q;
 times(&tms);
 q = (double)tms.tms_utime / (double)CLK_TCK;
 return q;
}
#endif

/* ---------- End of clinpack.c, Say goodnight Sandra! ---------- */

