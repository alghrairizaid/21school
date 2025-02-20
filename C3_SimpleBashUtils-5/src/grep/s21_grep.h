#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Structure to hold command-line arguments and pattern information */
typedef struct {
  char* pattern;
  size_t len_pattern;
  size_t mem_pattern;
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
} arguments;

/* Adds a new pattern to the arguments structure */
void addPattern(arguments* arg, char* pattern);

/* Reads patterns from a file and adds them to the arguments structure */
void addPatternsFromFile(arguments* arg, char* filepath);

/* Parses command line arguments and fills the arguments structure accordingly
 */
arguments parseArguments(int argc, char** argv);

/* Used for printing special characters (e.g., control characters) */
char formatOutputChar(char ch);

/* Used to print lines of text or match results */
void printLine(char* line, int n);

/* Used to print lines of text or match results */
void printMatches(regex_t* re, char* line);

/* Processes text lines and files, performing search and filtering according to
 * the given parameters */
void processFile(arguments arg, char* path, regex_t* reg);

/* Executes the main work of the program, using previous functions for input and
 * output processing */
void processOutput(arguments* arg, int argc, char** argv);

#endif  // S21_GREP_H
