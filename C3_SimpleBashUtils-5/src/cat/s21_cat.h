#ifndef S21_CAT_H
#define S21_CAT_H

#include <stdio.h>
#include <string.h>

struct Flags {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
};

int parse_flags(struct Flags *status, int argc, char **argv, int *textname);

void process_files(struct Flags *status, char **filenames, int count);

void print_file(struct Flags *status, const char *filename);

#endif  // S21_CAT_H
