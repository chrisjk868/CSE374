/* mem_utils.c utilities to support memory management system.
   HW6, CSE 374
*/

#include "mem.h"
#include "mem_impl.h"

void check_heap() {
  if (!freelist) return;
  freeNode* currentNode = freelist;
  uintptr_t minsize = currentNode->size;

  while (currentNode != NULL) {
    if (currentNode->size < minsize) {
      minsize = currentNode->size;
    }
    if (currentNode->next != NULL) {
      assert((uintptr_t)currentNode <(uintptr_t)(currentNode->next));
      assert((uintptr_t)currentNode + currentNode->size + NODESIZE
              <(uintptr_t)(currentNode->next));
    }
    currentNode = currentNode->next;
  }
  // go through free list and check for all the things
  if (minsize == 0) print_heap( stdout);
  assert(minsize >= MINCHUNK);
}

void get_mem_stats(uintptr_t* total_size, uintptr_t* total_free,
                   uintptr_t* n_free_blocks) {
  *total_size = totalmalloc;
  *total_free = 0;
  *n_free_blocks = 0;

  freeNode* currentNode = freelist;
  while (currentNode) {
    *n_free_blocks = *n_free_blocks + 1;
    *total_free = *total_free + (currentNode->size + NODESIZE);
    currentNode = currentNode->next;
  }
}

void print_heap(FILE *f) {
  printf("Printing the heap\n");
  freeNode* currentNode = freelist;
  while (currentNode !=NULL) {
    fprintf(f, "%" PRIuPTR, (uintptr_t)currentNode);
    fprintf(f, ", size: %" PRIuPTR, currentNode->size);
    fprintf(f, ", next: %" PRIuPTR, (uintptr_t)currentNode->next);
    fprintf(f, "\n");
    currentNode = currentNode->next;
  }
}

void insertnode(freeNode* node) {
   if (freelist == NULL) {
      freelist = node;
      return;
   }

   freeNode* prev_node = NULL;
   freeNode* current_node = freelist;

   while(current_node != NULL && (uintptr_t) node > (uintptr_t) current_node) {
      prev_node = current_node;
      current_node = current_node->next;
   }

   if (current_node != NULL && (uintptr_t) node == (uintptr_t) current_node) {
    // uh oh, trying to free memory that's already been freed.
    return;
   }

   if (!prev_node) { // node to insert belongs at the very start of freelist
      node->next = current_node;
      freelist = node;
   } else {
      prev_node->next = node;
      node->next = current_node;
   }
}
