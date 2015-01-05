#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fstream>

// IBIT should be smaller than or equal to 6
#define IBIT 4 
#define OBIT 4
#define IALL (1 << IBIT) 
#define OALL (1 << OBIT)
#define MAX (1 << IALL)
using namespace std;

typedef unsigned long long word;
word one = 1;
word mask = MAX-1;

word* arr = NULL;
char* map = NULL;
int* tag = NULL;
char* traO = NULL;
long* traA = NULL;
long* traB = NULL;
long count = 0;

char color = 1;

void init(){
   if(arr == NULL)
      arr = new word[MAX];
   if(map == NULL)
      map = new char[MAX];
   if(tag == NULL)
      tag = new int[MAX];

   if(traO == NULL)
      traO = new char[MAX];
   if(traA == NULL)
      traA = new long[MAX];
   if(traB == NULL)
      traB = new long[MAX];

   color++;
   count = 0;
}

void push(word a, long iA, long iB, char oper, word pos[OBIT], int& num){
   assert( count < MAX );

   //for(int i = 0; i < count; i++)
   //   if(arr[i] == a) return;
   if(map[a] == color)
      return;
   if(map[a] == -1)
      pos[num++] = count;

      
   map[a] = color;

   arr[count] = a;
   traO[count] = oper;
   traA[count] = iA;
   traB[count] = iB;

   count++;
}

void print2(word a){
   for(int i = IALL-1; i >= 0; i--)
      cout << ((a >> i) & 1);
   cout << endl;
}

void find(word target[OBIT], word pos[OBIT]){

   init();

   for(int i = 0; i < OBIT; ++i){
      printf("target[%d] = 0b", i);
      print2(target[i]);
   }

   // a1 ~ a6
   word a[IBIT] = {0};
   for(word i = 0; i < IALL ; ++i)
      for(int j = 0; j < IBIT; ++j)
         a[j] |= ((i >> j) & one) << i; 

   for(int i = 0; i < IBIT; ++i){
      int tmp;
      push(a[i], i, i, 'a', NULL, tmp);
      
      printf("a[%d] = 0b", i);
      print2(a[i]);
   }

   word tmp;
   long tmp_count;
   int num = 0;
   
   for(int i = 0; i < MAX; ++i)
      map[i] = 0;
   // mark target.
   for(int i = 0; i < OBIT; i++)
      map[target[i]] = -1;
   
   while(true){
      tmp_count = count;

      for(long i = 0; i < tmp_count; ++i)
         for(long j = i+1; j < tmp_count; ++j){
            tmp = arr[i] & arr[j];
            push(tmp, i, j, '&', pos, num);

            tmp = arr[i] | arr[j];
            push(tmp, i, j, '|', pos, num);

            tmp = arr[i] ^ arr[j];
            push(tmp, i, j, '^', pos, num);

            if(num == OBIT)return;
         }

      for(long i = 0; i < tmp_count; ++i){
         tmp = (~arr[i]) & mask;
         push(tmp, i, i, '~', pos, num);

         if(num == OBIT) return;
      }
   }
}

int getID(){
   static int i = 0;
   return i++;
}

int backtrace(FILE *fd, word pos){
   if(map[pos] == color) return tag[pos];
   map[pos] = color;
   tag[pos] = getID();

   char tag1[1024], tag2[1024];

   switch(traO[pos]){
      case '&':
      case '|':
      case '^':
         if(traO[traA[pos]] != 'a')
            sprintf(tag1, "x[%d]", backtrace(fd, traA[pos]));
         else
            sprintf(tag1, "a[%ld]", traA[traA[pos]]);
            
         if(traO[traB[pos]] != 'a')
            sprintf(tag2, "x[%d]", backtrace(fd, traB[pos]));
         else
            sprintf(tag2, "a[%ld]", traA[traB[pos]]);

         fprintf(fd, "   x[%d] = %s %c %s;\n", tag[pos], tag1, traO[pos], tag2);
         break;
      case '~':
         if(traO[traA[pos]] != 'a')
            sprintf(tag1, "x[%d]", backtrace(fd, traA[pos]));
         else
            sprintf(tag1, "a[%ld]", traA[traA[pos]]);

         fprintf(fd, "   x[%d] = ~%s; \n", tag[pos], tag1);
         break;
      case 'a':
         fprintf(fd, "   x[%d] = a[%ld];\n", tag[pos], traA[pos]); 
         break;
      default:
         assert( false );
   }
   return tag[pos];
}

void usage(char* progName){
   printf("usage: %s [-s sbox_file] sboxBit.h sboxArr.h\n", progName);
   printf("\n");
   printf("  Use sbox_file to generate BitSlicing and Array version of implementation\n");
   printf("  If no sbox_file is specified, use random permutation.\n");
   exit(-1);
}

int main(int argc, char* argv[]){

   const char* bitFn = NULL;
   const char* arrFn = NULL;
   const char* sbxFn = NULL;

   FILE* bitFILE = NULL;
   FILE* arrFILE = NULL;

   word SBOX[IALL];
   word target[OBIT] = {0};

   srand(time(NULL));

   // parse arguements
   for(int i = 1; i < argc; ++i){
      if(argv[i][0] == '-'){
         if(argv[i][1] == 's'){
            if(i + 1 >= argc) usage(argv[0]);
            sbxFn = argv[i+1]; 
            ++i;
            continue;
         } else
            usage(argv[0]);
      }else if(bitFn == NULL)
         bitFn = argv[i];
      else if(arrFn == NULL)
         arrFn = argv[i];
      else
         usage(argv[0]);
   }
   bitFILE = fopen(bitFn, "w");
   arrFILE = fopen(arrFn, "w");
   if(bitFILE == NULL || arrFILE == NULL)
      usage(argv[0]);

   // there's sbox 
   if(sbxFn){
      ifstream fin(sbxFn);
      for(word i = 0; i < IALL; ++i){
         if(!(fin >> SBOX[i])) usage(argv[0]);
         if(SBOX[i] > OALL) usage(argv[0]);
      }

   }else{
      for(word i = 0; i < IALL; ++i)
         SBOX[i] = i%OALL;

      word tmp;
      word a;
      for(word i = 0; i < IALL; ++i){
         a = rand() % IALL;
         tmp = SBOX[a];
         SBOX[a] = SBOX[i];
         SBOX[i] = tmp;
      }
   }

   // writeArray version
   {
      fprintf(arrFILE, "int sboxArr(int in){\n");
      fprintf(arrFILE, "   static int sbox[%d] = {\n", IALL);
      fprintf(arrFILE, "      ");
      for(word i = 0; i < IALL-1; ++i)
         fprintf(arrFILE, "%lld, ", SBOX[i]);
      fprintf(arrFILE, "%lld\n", SBOX[IALL-1]);
      fprintf(arrFILE, "   };\n");
      
      fprintf(arrFILE, "   return sbox[in];\n");
      fprintf(arrFILE, "}\n");
   }

   // write BitSlice version

   {
      fprintf(bitFILE, "void sboxBit(unsigned long a[4], unsigned long out[4]){\n");
      fprintf(bitFILE, "   static unsigned long x[1000];\n");
   }
   // roll all SBOX into target
   for(int i = 0; i < OBIT; ++i)
      for(word k = 0; k < IALL; ++k)
         target[i] |= ((SBOX[k] >> i) & one) << k;

   word pos[OBIT];
   find(target, pos);

   // need to permute
   word perm[OBIT];
   for(int i = 0; i < OBIT; ++i){
      int tmp = -1;
      for(int j = 0; j < OBIT; ++j)
         if(arr[pos[j]] == target[i])
            tmp = j;
      assert(tmp >= 0);
      perm[i] = tmp;
   }

   color++;
   for(int i = OBIT-1; i >= 0; --i){
      int tagID = backtrace(bitFILE, pos[perm[i]]);
      fprintf(bitFILE, "   out[%d] = x[%d]; \n", i, tagID);
   }
   fprintf(bitFILE, "}\n");

}
