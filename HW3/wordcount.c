/*
Copyright 2022 Christopher Ku

Christopher Ku 11-03-2022
CSE 374 2022 Autumn Homework 3 - Basic C
wordcount.c

This file closely follows the functionality of linux's builtin 'wc' command, which gets a summary
of the total number of lines, words, and character count of each file passed in as arguements. In
addition it also allows special flags that extracts specifc line, word or character count information
for different files.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Function that takes in a character and check if it is either a new line, tab or a space.
Returns true if that is the case or else returns false.
*/
int is_empty(char curr) {
  if (curr == ' ' || curr == '\n' || curr == '\t') {
    return 1;
  }
  return 0;
}

/*
Takes in the a file and reads each line indiviudally processing lines, words and character
counts. Returns the total number of lines present in the current file.
*/
int line_summary(FILE *file, char* f_name, char* flag) {
  char buffer[500] = {0};
  // Total number of lines
  int l_count = 0;
  // Total number of words
  int w_count = 0;
  // Total number of characters
  int c_count = 0;
  while (fgets(buffer, 500, file)) {
    l_count++;
    c_count += strlen(buffer);
    int i;
    char prev = ' ';
    for (i = 0; buffer[i] != '\0'; i++) {
      if (is_empty(prev) && !is_empty(buffer[i])) {
        w_count++;
      }
      prev = buffer[i];
    }
  }
  if (flag != NULL) {
    if (strcmp(flag, "-l") == 0) {
      fprintf(stdout, "%i\n", l_count);
    } else if (strcmp(flag, "-w") == 0) {
      fprintf(stdout, "%i\n", w_count);
    } else if (strcmp(flag, "-c") == 0) {
      fprintf(stdout, "%i\n", c_count);
    }
  } else {
    fprintf(stdout, "%i %i %i %s\n", l_count, w_count, c_count, f_name);
  }
  return l_count;
}

int main(int argc, char *argv[argc + 1]) {
  // Total number of lines from every file accumulated
  int total_lines = 0;
  int i;
  // '-l', '-w' '-c' flags for input argument
  char* flag = NULL;
  if (argc < 2) {
    fprintf(stderr, "Usage: %s requires an input file\n", argv[0]);
    return EXIT_FAILURE;
  }
  if (strcmp(argv[1], "-l") == 0 ||
      strcmp(argv[1], "-w") == 0 ||
      strcmp(argv[1], "-c") == 0) {
    flag = argv[1];
  }
  if (flag != NULL && argc == 2) {
    fprintf(stderr, "Usage: %s requires an input file\n", argv[0]);
    return EXIT_FAILURE;
  }
  for (i = 1; i < argc; i++) {
    char* f_name = argv[i];
    FILE* file = fopen(f_name, "r");
    if (i == 1 && flag != NULL) {
      continue;
    }
    if (file) {
      total_lines += line_summary(file, f_name, flag);
      fclose(file);
    } else {
      fprintf(stderr, "NON FILE will not open.  Skipping.\n");
    }
  }
  if (flag == NULL) {
    fprintf(stdout, "Total Lines = %i\n", total_lines);
  }
  return EXIT_SUCCESS;
}
