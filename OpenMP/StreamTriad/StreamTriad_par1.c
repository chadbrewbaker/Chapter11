#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "timer.h"

int main(int argc, char *argv[]){

   int nsize = 20000000, ntimes=16;
   double a[nsize];
   double b[nsize];
   double c[nsize];

   struct timespec tstart;
   // initializing data and arrays
   double scalar = 3.0, time_sum = 0.0;
#pragma omp target teams distribute parallel for simd
   for (int i=0; i<nsize; i++) {
      a[i] = 1.0;
      b[i] = 2.0;
   }

   for (int k=0; k<ntimes; k++){
      cpu_timer_start(&tstart);
      // stream triad loop 
#pragma omp target teams distribute parallel for simd
      for (int i=0; i<nsize; i++){
         c[i] = a[i] + scalar*b[i];
      }
      time_sum += cpu_timer_stop(tstart);
   }

   printf("Average runtime for stream triad loop is %lf secs\n", time_sum/ntimes);

   return(0);
}
