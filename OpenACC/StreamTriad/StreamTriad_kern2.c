#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "timer.h"

void StreamTriad(double *restrict c, double *restrict a, double *restrict b, double scalar, int nsize);

int main(int argc, char *argv[]){

   int nsize = 20000000, ntimes=16;
   double *a = malloc(nsize * sizeof(double));
   double *b = malloc(nsize * sizeof(double));
   double *c = malloc(nsize * sizeof(double));

#pragma acc enter data create(a[0:nsize],b[0:nsize],c[0:nsize])

   struct timespec tstart;
   // initializing data and arrays
   double scalar = 3.0, time_sum = 0.0;
#pragma acc kernels present(a[0:nsize],b[0:nsize])
   {
#pragma acc loop independent
      for (int i=0; i<nsize; i++) {
         a[i] = 1.0;
         b[i] = 2.0;
      }
   }

   for (int k=0; k<ntimes; k++){
      cpu_timer_start(&tstart);
      // stream triad loop 
#pragma acc kernels present(a[0:nsize],b[0:nsize],c[0:nsize])
      {
#pragma acc loop independent
         for (int i=0; i<nsize; i++){
            c[i] = a[i] + scalar*b[i];
         }
      }
      time_sum += cpu_timer_stop(tstart);
   }

   printf("Average runtime for stream triad loop is %lf msecs\n", time_sum/ntimes);

#pragma acc exit data delete(a[0:nsize],b[0:nsize],c[0:nsize])

   free(a);
   free(b);
   free(c);

   return(0);
}
