#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include "sboxArr.h"
#include "sboxBit.h"

using namespace std;

// one 4x4 sbox
//void sboxBit(unsigned long a[4], unsigned long out[4]);
//int sboxArr(int in);

void print2(unsigned long a){
   for(int i = 15; i >= 0; i--)
      cout << ((a >> i) & 1);
   cout << endl;
}


struct timeval	start_tv, end_tv;

void tic(){
   gettimeofday (&start_tv, NULL);
}

double toc(){
   gettimeofday (&end_tv, NULL);

   int t;
   double td;

   t = (end_tv.tv_sec - start_tv.tv_sec) * 1000000
      + (end_tv.tv_usec - start_tv.tv_usec);
   td = (double) t / 1000000.0;
   return td;
}

int main(int argc, char* argv[]){
   long n = 1L << 30;
   double td1, td2;
   int arr[64];

   int ansArr[64];
   int ansBit[64] = {0};
   unsigned long a[4] = {0};
   unsigned long out[4];
   unsigned long one = 1;

   for(int i = 0; i < 64; ++i)
      arr[i] = i%16;
   
   // roll in
   for(int i = 0; i < 4; ++i)
      for(int j = 0; j < 64; ++j)
         a[i] |= ((arr[j] >> i) & one) << j;


   tic();
   for(long i = 0; i < n/64; ++i)
      sboxBit(a, out);
   td1 = toc();
   printf("BitSlice: %e times per second.\n", n/td1);

   // roll out
   for(int i = 0; i < 64; ++i){
      for(int j = 0; j < 4; ++j)
         ansBit[i] |= ((out[j] >> i) & one) << j;
   }

   tic();
   for(long i = 0; i < n; ++i)
      ansArr[0] = sboxArr(arr[0]);
   td2 = toc();
   printf("Array:    %e times per second.\n", n/td2);

   printf("\n");
   printf("BitSlice is %lf times faster than Array.\n", td2/td1);
   printf("\n");

   // testing accuracy

   for(int i = 0; i < 64; ++i)
      ansArr[i] = sboxArr(arr[i]);

   for(int i = 0; i < 64; ++i)
      if(ansArr[i] != ansBit[i]){
         printf("error!\n");
         return 0;
      }
   
   printf("pass test!!!!\n");

   return 0;
}
