/* freemem.c
   implements freemem (free) for memory system
   CSE 374 HW6
*/

#include <assert.h>
#include "mem.h"
#include "mem_impl.h"

void mergemem() {
   if (freelist == NULL) {
      return;
   }

   freeNode* current_node = freelist;
   while(current_node != NULL && current_node->next != NULL) {
      uintptr_t current_end_addr = (uintptr_t) current_node + current_node->size + NODESIZE;
      uintptr_t next_start_addr = (uintptr_t) current_node->next;
      uintptr_t next_end_addr = (uintptr_t) current_node->next + current_node->next->size + NODESIZE;
      if (current_end_addr >= next_start_addr) {
         if (next_end_addr - (uintptr_t) current_node - NODESIZE > current_node->size) {
            current_node->size = next_end_addr - (uintptr_t) current_node - NODESIZE;
         }
         current_node->next = current_node->next->next;
      } else {
         current_node = current_node->next;
      }
   }
}

void freemem(void* p) {
   freeNode* node_to_insert = (freeNode*) ((uintptr_t) p - NODESIZE);
   insertnode(node_to_insert);
   mergemem();
}



