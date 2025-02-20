#include "s21_cat.h"

int main(int argc, char **argv) {
  struct Flags status = {0};
  int textname = 0;

  if (parse_flags(&status, argc, argv, &textname) != 0) {
    fprintf(stderr, "cat: illegal option -- -\n");
    fprintf(stderr, "usage: %s [-benst] [file ...]\n", argv[0]);
    return 1;
  }

  while (textname < argc) {
    print_file(&status, argv[textname]);
    textname++;
  }

  return 0;
}

int parse_flags(struct Flags *status, int argc, char **argv, int *textname) {
  int flag_error_count = 0;
  int i;

  for (i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (strcmp(argv[i], "--") == 0) {
        *textname = i + 1;
        break;
      } else if (strcmp(argv[i], "--number-nonblank") == 0) {
        status->b = 1;
      } else if (strcmp(argv[i], "--number") == 0) {
        status->n = 1;
      } else if (strcmp(argv[i], "--squeeze-blank") == 0) {
        status->s = 1;
      } else {
        for (size_t j = 1; j < strlen(argv[i]); j++) {
          switch (argv[i][j]) {
            case 'b':
              status->b = 1;
              break;
            case 'e':
              status->e = 1;
              status->v = 1;
              break;
            case 'E':
              status->e = 1;
              break;
            case 'n':
              status->n = 1;
              break;
            case 's':
              status->s = 1;
              break;
            case 't':
              status->t = 1;
              status->v = 1;
              break;
            case 'T':
              status->t = 1;
              break;
            case 'v':
              status->v = 1;
              break;
            default:
              flag_error_count++;
          }
        }
      }
    } else {
      *textname = i;
      break;
    }
  }

  return flag_error_count;
}

void process_files(struct Flags *status, char **filenames, int count) {
  for (int i = 0; i < count; i++) {
    print_file(status, filenames[i]);
  }
}

void print_file(struct Flags *status, const char *filename) {
  FILE *file = fopen(filename, "rt");

  if (file != NULL) {
    int line_count = 1;
    int empty_line_count = 0;
    int previous_char = '\n';

    while (1) {
      int current_char = fgetc(file);

      if (current_char == EOF) break;

      if (status->s) {
        if (current_char == '\n' && previous_char == '\n') {
          empty_line_count++;
          if (empty_line_count > 1) {
            previous_char = current_char;
            continue;
          }
        } else {
          empty_line_count = 0;
        }
      }

      if (previous_char == '\n' &&
          (((status->b && current_char != '\n') && status->n) ||
           (status->b && current_char != '\n') ||
           (status->b == 0 && status->n))) {
        printf("%6d\t", line_count++);
      }

      if (status->t && current_char == '\t') {
        printf("^");
        current_char = 'I';
      }

      if (status->e && current_char == '\n') {
        if (status->b && previous_char == '\n') {
          printf("      \t$");
        } else {
          printf("$");
        }
      }

      if (status->v && current_char != '\n') {
        if (current_char >= 0 && current_char < 32 && current_char != 9 &&
            current_char != 10) {
          printf("^");
          current_char += 64;
        } else if (current_char > 127 && current_char < 160) {
          printf("M-^");
          current_char -= 64;
        } else if (current_char == 127) {
          printf("^");
          current_char -= 64;
        }
      }

      printf("%c", current_char);
      previous_char = current_char;
    }

    fclose(file);
  } else {
    fprintf(stderr, "%s: No such file or directory\n", filename);
  }
}
