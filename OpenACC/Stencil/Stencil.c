#include <stdio.h>
#include <stdlib.h>

#include "malloc2D.h"
#include "timer.h"

#define SWAP_PTR(xnew,xold,xtmp) (xtmp=xnew, xnew=xold, xold=xtmp)

int main(int argc, char *argv[])
{
   struct timespec tstart_cpu, tstop_cpu;
   double cpu_time;
   int imax=2002, jmax = 2002;
   int niter=1000, nburst=100;

   double** restrict xtmp;
   double** restrict x    = malloc2D(jmax, imax);
   double** restrict xnew = malloc2D(jmax, imax);

   for (int j = 0; j < jmax; j++){
      for (int i = 0; i < imax; i++){
         xnew[j][i] = 0.0;
         x[j][i]    = 5.0;
      }
   }

   for (int j = jmax/2 - 5; j < jmax/2 + 5; j++){
      for (int i = imax/2 - 5; i < imax/2 -1; i++){
         x[j][i] = 400.0;
      }
   }

   for (int iter = 0; iter < niter; iter+=nburst){

      for (int ib = 0; ib < nburst; ib++){
         cpu_timer_start(&tstart_cpu);
         for (int j = 1; j < jmax-1; j++){
            for (int i = 1; i < imax-1; i++){
               xnew[j][i] = ( x[j][i] + x[j][i-1] + x[j][i+1] + x[j-1][i] + x[j+1][i] )/5.0;
            }
         }
         cpu_time += cpu_timer_stop(tstart_cpu);

         SWAP_PTR(xnew, x, xtmp);
      }

      printf("Iter %d\n",iter+nburst);
   }

   free(x);
   free(xnew);

   printf("Timing is %lf\n",cpu_time);
}
