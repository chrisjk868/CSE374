/* getmem.c
   implements getmem (malloc) for memory system
   CSE 374 HW6
*/

#include <assert.h>
#include "mem.h"
#include "mem_impl.h"

#define SPLIT_THRESHOLD 8000

freeNode* freelist = NULL;
uintptr_t totalmalloc = 0;

/* Takes a freeNode and splits it into two blocks connected blocks, the second
   of which is of size target_size.

   Example node given:
   (node size 8048) -> (node size 80) 

   Given target_size 32, returns the following node:
   (desired node size 32) -> (remainder node size 8016) -> (node size 80) */
freeNode* splitblock(freeNode* node, uintptr_t target_size) {
  uintptr_t remainder_node_size = node->size - NODESIZE - target_size;
  node->size = target_size;
  freeNode* remainder_node = (freeNode*) ((uintptr_t) node + node->size + NODESIZE);
  remainder_node->size = remainder_node_size;
  remainder_node->next = node->next;
  node->next = remainder_node;
  return node;
}

/* Define your functions below: */

void* getmem(uintptr_t size) {
  /* make sure you return a pointer to the usable memory that
     is at least 'size' bytes long.
     To get you started we are 'stubbing in' a call that will
     return a usable value.  You will replace this code. */
  if (size <= 0) {
      return NULL;
  }

  check_heap();

  // ensure size is a multiple of 16
  if (size % 16 != 0) {
    size = size - (size % 16) + 16;
  }

  /* Iterate through the freelist looking for a suitable block to return. 
     Three scenarios:
     1. We find a block of size "cur_block_size" big enough where (cur_block_size - size < SPLIT_THRESHOLD):
        - We don't need to split the cur_block. Remove this block from freelist and return it.
     2. We find a block big enough where (cur_block_size - size >= SPLIT_THRESHOLD):
        - Split this block, remove the smaller chunk from the freelist and return it. 
     3. Block isn't big enough (cur_block_size < size). Keep iterating.

     If we reach the end of the list without returning. This means that either no blocks in
     freelist were large enough, or freelist is empty. In this case:
        - If (size > BIGCHUNK): malloc size + size label space, return this chunk.
        - Otherwise, malloc BIGCHUNK, split it, return the smaller split chunk to user.*/

  freeNode* current_node = freelist;
  freeNode* prev = NULL;

  while (current_node) {
    if (current_node->size < size) { // if we haven't found a big enough node, continue.
      prev = current_node;
      current_node = current_node->next;
      continue;
    }
    if (current_node->size - size < SPLIT_THRESHOLD) { // we found a big enough node!
      if (!prev) {
        freelist = current_node->next;
      } else {
        prev->next = current_node->next;
      }
      return (void*) current_node + NODESIZE;
    } else {
      // Block is too big. We need to split.
      freeNode* block_to_return = splitblock(current_node, size);
      if (!prev) {
        freelist = block_to_return->next;
      } else {
        prev->next = block_to_return->next;
      }
      return (void*) block_to_return + NODESIZE;
    }
  }
  // if we've reached this point, there was no suitable block in freelist to return.

  freeNode* new_node = NULL;
  if (size >= BIGCHUNK - MINCHUNK - NODESIZE) { // size is too large, directly malloc it
    new_node = (freeNode*) malloc(size + NODESIZE);
    totalmalloc += size + NODESIZE;
    new_node->size = size;
    new_node->next = NULL;
  } else {
    new_node = (freeNode*) malloc(BIGCHUNK + NODESIZE);
    totalmalloc += BIGCHUNK + NODESIZE;
    new_node->size = BIGCHUNK;
    new_node->next = NULL;
    new_node = splitblock(new_node, size);
    insertnode(new_node->next);
  }
  check_heap();
  return (void*) new_node + NODESIZE;
}