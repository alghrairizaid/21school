#define _GNU_SOURCE
#include "s21_grep.h"

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Adds a new pattern to the arguments structure */
void addPattern(arguments* arg, char* pattern) {
  int n = strlen(pattern);

  // Allocate initial memory for the pattern if it's the first pattern
  if (arg->len_pattern == 0) {
    arg->pattern = calloc(1024, sizeof(char));
    arg->mem_pattern = 1024;
  }

  // Reallocate memory if the current pattern length plus new pattern length
  // exceeds allocated memory
  if (arg->mem_pattern < arg->len_pattern + n) {
    arg->pattern = realloc(arg->pattern, arg->mem_pattern * 2);
    arg->mem_pattern *= 2;
  }

  // Append a "|" if it's not the first pattern to separate multiple patterns
  if (arg->len_pattern != 0) {
    strcat(arg->pattern + arg->len_pattern, "|");
    arg->len_pattern++;
  }

  // Add the new pattern enclosed in parentheses
  arg->len_pattern += sprintf(arg->pattern + arg->len_pattern, "(%s)", pattern);
}

/* Reads patterns from a file and adds them to the arguments structure */
void addPatternsFromFile(arguments* arg, char* filepath) {
  FILE* f = fopen(filepath, "r");

  // Handle file opening error
  if (f == NULL) {
    if (!arg->s) perror(filepath);
    exit(1);
  }

  char* line = NULL;
  size_t memline = 0;
  ssize_t read;

  // Read each line from the file and add it as a pattern
  while ((read = getline(&line, &memline, f)) != -1) {
    if (line[read - 1] == '\n') line[read - 1] = '\0';
    addPattern(arg, line);
  }

  free(line);
  fclose(f);
}

/* Parses command line arguments and fills the arguments structure accordingly
 */
arguments parseArguments(int argc, char** argv) {
  arguments arg = {0};  // Initialize arguments structure
  int opt;

  // Process command line options
  while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) != -1) {
    switch (opt) {
      case 'e':
        arg.e = 1;
        addPattern(&arg, optarg);
        break;
      case 'i':
        arg.i = REG_ICASE;
        break;
      case 'v':
        arg.v = 1;
        break;
      case 'c':
        arg.c = 1;
        break;
      case 'l':
        arg.c = 1;
        arg.l = 1;
        break;
      case 'n':
        arg.n = 1;
        break;
      case 'h':
        arg.h = 1;
        break;
      case 's':
        arg.s = 1;
        break;
      case 'f':
        arg.f = 1;
        addPatternsFromFile(&arg, optarg);
        break;
      case 'o':
        arg.o = 1;
        break;
    }
  }

  // If no pattern is specified, use the first non-option argument as the
  // pattern
  if (arg.len_pattern == 0) {
    addPattern(&arg, argv[optind]);
    optind++;
  }

  // If only one argument remains, set the h option
  if (argc - optind == 1) {
    arg.h = 1;
  }

  return arg;
}

/* Used for printing special characters (e.g., control characters) */
char formatOutputChar(char ch) {
  if (ch == '\n' || ch == '\t') return ch;
  if (ch < 0) {
    printf("M-");
    ch = ch & 0x7F;
  }
  if (ch > 0 && ch <= 31) {
    putchar('^');
    ch *= 64;
  } else if (ch == 127) {
    putchar('^');
    ch = '?';
  }
  return ch;
}

/* Used to print lines of text or match results */
void printLine(char* line, int n) {
  for (int i = 0; i < n; i++) {
    putchar(line[i]);
  }
  if (line[n - 1] != '\n') putchar('\n');
}

/* Used to print lines of text or match results */
void printMatches(regex_t* re, char* line) {
  regmatch_t match;
  int offset = 0;

  // Find and print all matches in the line
  while (1) {
    int result = regexec(re, line + offset, 1, &match, 0);
    if (result != 0) {
      break;
    }
    for (int i = match.rm_so; i < match.rm_eo; i++) {
      putchar(line[offset + i]);
    }
    putchar('\n');
    offset += match.rm_eo;
  }
}

/* Processes text lines and files, performing search and filtering according to
 * the given parameters */
void processFile(arguments arg, char* path, regex_t* reg) {
  FILE* f = fopen(path, "r");

  // Handle file opening error
  if (f == NULL) {
    if (!arg.s) perror(path);
    exit(1);
  }

  char* line = NULL;
  size_t memline = 0;
  ssize_t read;
  int matchCount = 0;
  int lineCount = 1;

  // Read each line from the file and process it
  while ((read = getline(&line, &memline, f)) != -1) {
    int result = regexec(reg, line, 0, NULL, 0);
    if ((result == 0 && !arg.v) || (arg.v && result != 0)) {
      if (!arg.c && !arg.l) {
        if (!arg.h) printf("%s:", path);
        if (arg.n) printf("%d:", lineCount);
        if (arg.o) {
          printMatches(reg, line);
        } else {
          printLine(line, read);
        }
      }
      matchCount++;
    }
    lineCount++;
  }

  free(line);

  // Print counts or file names as needed
  if ((arg.c && !arg.l) || (arg.l && !arg.c)) {
    if (!arg.h) printf("%s:", path);
    printf("%d\n", matchCount);
  }
  if (arg.l && matchCount > 0) printf("%s\n", path);

  fclose(f);
}

/* Executes the main work of the program, using previous functions for input and
 * output processing */
void processOutput(arguments* arg, int argc, char** argv) {
  regex_t re;
  int error = regcomp(&re, arg->pattern, REG_EXTENDED | arg->i);

  // Handle regex compilation error
  if (error) perror("Error");

  // Process each file specified in the arguments
  for (int i = optind; i < argc; i++) {
    processFile(*arg, argv[i], &re);
  }

  regfree(&re);
}

/* Main entry point of the program, calls parseArguments and then processOutput
 */
int main(int argc, char** argv) {
  arguments arg = parseArguments(argc, argv);
  processOutput(&arg, argc, argv);
  free(arg.pattern);
  return 0;
}
