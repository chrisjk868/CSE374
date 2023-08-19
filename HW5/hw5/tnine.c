/*
  Christopher Ku
  02-12-2022
  CSE 374 HW5

  tnine is a program that drives a trie / t9 program.  This code
  will build a trie, according to trienode.  It will also run
  an interactive session where the user can retrieve words using
  t9 sequences.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "trienode.h"

extern trieNode* curr_node;

// run_session run the on-going decoding interaction with the
// user.  It requires a previously build wordTrie to work.
void run_session(trieNode *wordTrie);

int main(int argc, char **argv) {
  FILE* dictionary = NULL;
  trieNode* wordTrie = NULL;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s [DICTIONARY]\n", argv[0]);
    return EXIT_FAILURE;
  } else {
    dictionary = fopen(argv[1], "r");
    if (!dictionary) {
      fprintf(stderr, "Error: Cannot open %s\n", argv[1]);
      return EXIT_FAILURE;
    }
  }

  wordTrie = build_tree(dictionary);

  run_session(wordTrie);

  free_tree(wordTrie);
  
  return(EXIT_SUCCESS);
}

// takes in a sentinel Trie node and runs an interactive session
// with the user until they exit.
void run_session(trieNode *wordTrie) {
  printf("Enter \"exit\" to quit.\n");
  while (1) {
    char input[MAXLEN];
    char exit[] = "exit";
    printf("Enter Key Sequence (or \"#\" for next word):");
    printf("\n> ");
    scanf("%s", &input);
    if (strcmp(input, exit) == 0) {
      break;
    } else if (strcmp(input, "#") == 0) {
      if (curr_node->branches[1] != NULL) {
        curr_node = curr_node->branches[1];
        printf("\'%s\'\n", curr_node->word);
      } else {
        printf("There are no more T9onyms\n");
      }
    } else {
      char* word = get_word(wordTrie, input);
      if (word == NULL) {
        printf("Not found in current dictionary.\n");
      } else {
        if (strcmp(word, "-1") == 0) {
          printf("There are no more T9onyms\n");
        } else {
          printf("\'%s\'\n", word);
        }
      }
    }
  }
}