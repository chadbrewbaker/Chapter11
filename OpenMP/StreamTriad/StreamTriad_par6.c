#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "timer.h"

int main(int argc, char *argv[]){

   int nsize = 20000000, ntimes=16;

   double *a = omp_target_alloc(nsize*sizeof(double), omp_get_default_device());
   double *b = omp_target_alloc(nsize*sizeof(double), omp_get_default_device());
   double *c = omp_target_alloc(nsize*sizeof(double), omp_get_default_device());

   struct timespec tstart;
   // initializing data and arrays
   double scalar = 3.0, time_sum = 0.0;
#pragma omp target teams distribute parallel for simd is_device_ptr(a, b)
   for (int i=0; i<nsize; i++) {
      a[i] = 1.0;
      b[i] = 2.0;
   }

   for (int k=0; k<ntimes; k++){
      cpu_timer_start(&tstart);
      // stream triad loop 
#pragma omp target teams distribute parallel for simd is_device_ptr(a, b, c)
      for (int i=0; i<nsize; i++){
         c[i] = a[i] + scalar*b[i];
      }
      time_sum += cpu_timer_stop(tstart);
   }

   printf("Average runtime for stream triad loop is %lf msecs\n", time_sum/ntimes);

   omp_target_free(a, omp_get_default_device());
   omp_target_free(b, omp_get_default_device());
   omp_target_free(c, omp_get_default_device());

   return(0);
}
