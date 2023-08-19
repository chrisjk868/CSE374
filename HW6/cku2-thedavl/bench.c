/* bench.c is the benchmarking /test program for mem memory management
   bench.c is implemented as part of HW6, CSE374 22WI
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "mem.h"

/* fill_mem is a helper function that takes in an address and the
   size of the memory allocation at that address.  It should fill
   the first 16 bytes (or up to size) with hexedecimal values.*/
void fill_mem(void* ptr, uintptr_t size);

// Reports the current status of used, freed memory blocks, size
// and time
void print_stats(double time, uintptr_t tot_size,
            uintptr_t tot_free, uintptr_t free_blocks);

/* Synopsis:   bench (main)
   [ntrials] (10000) getmem + freemem calls
   [pctget] (50) % of calls that are get mem
   [pctlarge] (10) % of calls requesting more memory than lower limit
   [small_limit] (200) largest size in bytes of small block
   [large_limit] (20000) largest size in byes of large block
   [random_seed] (time) initial seed for randn
*/
int main(int argc, char** argv ) {
  // Initialize the parameters
  int NTRIALS;
  int PCTGET;
  int PCTLARGE;
  int SMALL_L;
  int LARGE_L;
  int RANDOM_SEED;

  (argc > 1) ? (NTRIALS = atoi(argv[1])) : (NTRIALS = 10000);
  (argc > 2) ? (PCTGET = atoi(argv[2])) : (PCTGET = 50);
  (argc > 3) ? (PCTLARGE = atoi(argv[3])) : (PCTLARGE = 10);
  (argc > 4) ? (SMALL_L = atoi(argv[4])) : (SMALL_L = 200);
  (argc > 5) ? (LARGE_L = atoi(argv[5])) : (LARGE_L = 20000);

  // initialize random number gen.
  (argc > 6) ? srand(atoi(argv[6])) : srand(time(NULL));

  printf("%d trials, %d%% getmem calls.\n", NTRIALS, PCTGET);

  void* blocks[NTRIALS];
  int ntrials = 0, nblocks = 0;
  double time;
  clock_t start, end;
  start = clock();
  srand(RANDOM_SEED);
  void ** mem_ptr;
  mem_ptr = (void **) malloc(NTRIALS * sizeof(void *));
  uintptr_t tot_size = 0;
  uintptr_t tot_free = 0;
  uintptr_t free_blocks = 0;
  int size = 0;
  int ten = 1;

  for (int i = 0; i < NTRIALS; i++) {
     int pct = rand() % 100;
     if (PCTGET > pct) {
      uintptr_t bsize;
      if (rand() % 100 < PCTLARGE) {
         bsize = SMALL_L + rand() % (LARGE_L - SMALL_L);
      } else {
         bsize = 1 + rand() % SMALL_L;
      }
      mem_ptr[size] = getmem(bsize);
      size++;
      } else {
         if (size > 0) {
            int n = rand() % (size);
            freemem(mem_ptr[n]);
            size--;
            mem_ptr[n] = mem_ptr[size];
         }
      }
      // report after 10, 20, ... 100
      if ((i + 1) * 10 / NTRIALS >= ten) {
         end = clock();
         time = ((double) (end - start)) / CLOCKS_PER_SEC;
         tot_free = 0, free_blocks = 0;
         get_mem_stats(&tot_size, &tot_free, &free_blocks);
         print_stats(time, tot_size, tot_free, free_blocks);
         ten++;
      }
  }
  return EXIT_SUCCESS;
}

// Takes in all statistics recorded from making calls to allocate
// and deallocate memory and reports relevant statistics
void print_stats(double time, uintptr_t tot_size,
            uintptr_t tot_free, uintptr_t free_blocks) {
  printf("Total CPU time in seconds:  %f\n", time);
  printf("Total storage acquired in bytes: %lu\n", tot_size);
  printf("Total number of free blocks: %lu\n", free_blocks);
  if (free_blocks == 0) {
    printf("Average number of free blocks in bytes: 0\n");
  } else {
    printf("Average number of free blocks in bytes: %lu\n",
           tot_free / free_blocks);
  }
  printf("\n");
  return;
}

// Stores hex value for 16 bytes for a given pointer
void fill_mem(void *ptr, size_t size) {
  if (size > 16) {
    size = 16;
  }

  for (int i = 0; i < size; i++) {
    unsigned char *curr = (unsigned char *) ((uintptr_t) ptr + i);
    *curr = 0xFE;
  }
}
