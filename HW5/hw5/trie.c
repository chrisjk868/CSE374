/* 
   Christopher Ku
   02-12-2022
   CSE 374 HW5

   trie implements a trie, made of trieNodes. This includes
   code to build, search, and delete a trie
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "trienode.h"

trieNode* curr_node;

// Returns an empty sentinel Trie node.
trieNode* create_trie(void);

// Returns the integer values of a character.
int get_index(char c);

// Inserts a given word into the Trie.
void insert_word(trieNode* root, char* word);

// Returns an empty Trie node.
trieNode* create_trie(void) {
   trieNode* node = (trieNode*)malloc(sizeof(trieNode));
   if (node == NULL) {
      fprintf(stderr, "Error: Malloc Failed\n");
      exit(1);
   }
   node->word = NULL;
   for (int i = 0; i < BRANCHES; i++) {
      node->branches[i] = NULL;
   }
   return node;
}


// Takes a character and returns its associated index within the
// branches of a Trie node. 
int get_index(char c) {
   if (c == 'a' || c == 'b' || c == 'c') {
      return 2;
   } else if (c == 'd' || c == 'e' || c == 'f') {
      return 3;
   } else if (c == 'g' || c == 'h' || c == 'i') {
      return 4;
   } else if (c == 'j' || c == 'k' || c == 'l') {
      return 5;
   } else if (c == 'm' || c == 'n' || c == 'o') {
      return 6;
   } else if (c == 'p' || c == 'q' || c == 'r' || c == 's') {
      return 7;
   } else if (c == 't' || c == 'u' || c == 'v') {
      return 8;
   } else if (c == 'w' || c == 'x' || c == 'y' || c == 'z') {
      return 9;
   } else {
      return 0;
   }
}

// Takes a Trie node and a word then inserts that given word into
// the provided Trie
void insert_word(trieNode* root, char* word) {
   trieNode* curr = root;
   for (int i = 0; i < strlen(word); i++) {
      int char_index = get_index(word[i]);
      if (curr->branches[char_index] == NULL) {
         curr->branches[char_index] = create_trie();
      }
      curr = curr->branches[char_index];
   }
   while (curr->word != NULL) {
      if (!curr->branches[1]) {
         curr->branches[1] = create_trie();
      }
      curr = curr->branches[1];
   }
   curr->word = malloc(strlen(word) + 1);
   strncpy(curr->word, word, strlen(word) + 1);
}

// Takes in a file and builds a Trie associated with the given
// words in the file
trieNode* build_tree(FILE* dict) {
   trieNode* root = create_trie();
   char buffer[50];
   while (fgets(buffer, 50, dict)) {
      buffer[strcspn(buffer, "\n")] = 0;
      char* word = buffer;
      insert_word(root, word);
   }
   fclose(dict);
   return root;
}

// Gvien an encoding it will return the word associated with the
// encoding.
char* get_word(trieNode* root, char* pattern) {
   trieNode* curr = root;
   curr_node = root;
   for (int i = 0; i < strlen(pattern); i++) {
      int char_index = pattern[i] - '0';
      if (pattern[i] == '#') {
         char_index = 1;
      }
      if (char_index < 1 || char_index > 9 || curr->branches[char_index] == NULL) {
         if (pattern[i] != '#') {
            return NULL;
         } else {
            return "-1";
         }
      }
      curr = curr->branches[char_index];
      curr_node = curr;
   }
   return curr->word;
}

// After allocating memory for building the tree this method would
// free the memory used for building the tree.
void free_tree(trieNode* root) {
   if (root == NULL) {
      return;
   }
   for (int i = 0; i < BRANCHES; i++) {
      free_tree(root->branches[i]);
   }
   free(root->word);
   free(root);
   return;
}