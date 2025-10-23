#include "s21_grep.h"

#include <errno.h>
#include <getopt.h>
#include <regex.h>
#include <string.h>

bool parse_options(int argc, char *argv[], GrepOptions *options) {
  int opt, res = 0;
  while ((opt = getopt(argc, argv, "e:ivclnhsf:oE")) != -1) {
    switch (opt) {
      case 'e':
        if (options->pattern_count >= MAX_PATTERNS) {
          fprintf(stderr, "Too many patterns\n");
          res = 1;
        }
        strncpy(options->patterns[options->pattern_count++], optarg,
                PATTERN_SIZE - 1);
        options->patterns[options->pattern_count - 1][PATTERN_SIZE - 1] = '\0';
        break;
      case 'i':
        options->ignore_case = true;
        break;
      case 'v':
        options->invert_match = true;
        break;
      case 'c':
        options->count = true;
        break;
      case 'l':
        options->files_with_matches = true;
        break;
      case 'n':
        options->line_numbers = true;
        break;
      case 'h':
        options->no_filenames = true;
        break;
      case 's':
        options->no_messages = true;
        break;
      case 'f':
        if (!read_patterns_from_file(optarg, options)) {
          res = 1;
        }
        break;
      case 'o':
        options->only_matching = true;
        break;
      case 'E':
        options->use_extended_regexp = true;
        break;
      default:
        fprintf(stderr, "Usage: %s [-eivclnhsfoE] pattern [file...]\n",
                argv[0]);
        res = 1;
    }
  }
  return res;
}

bool read_patterns_from_file(const char *filename, GrepOptions *options) {
  int res = 1;
  FILE *file = fopen(filename, "r");
  if (!file) {
    if (!options->no_messages) {
      fprintf(stderr, "%s: %s\n", filename, strerror(errno));
    }
    res = 0;
  }
  if (res) {
    char line[PATTERN_SIZE];
    while (fgets(line, sizeof(line), file)) {
      line[strcspn(line, "\n")] = '\0';
      if (line[0] != '\0') {
        if (options->pattern_count >= MAX_PATTERNS) {
          fprintf(stderr, "Too many patterns\n");
          fclose(file);
          res = 0;
        }
        if (res) {
          strncpy(options->patterns[options->pattern_count++], line,
                  PATTERN_SIZE - 1);
          options->patterns[options->pattern_count - 1][PATTERN_SIZE - 1] =
              '\0';
        }
      }
    }
  }

  fclose(file);
  return res;
}

void process_file(FILE *file, GrepOptions *options, const char *filename) {
  regex_t regex;
  char line[LINE_SIZE];
  int line_number = 0;
  int match_count = 0;
  int cflags = options->use_extended_regexp ? REG_EXTENDED : 0;
  int res = 1;

  if (options->ignore_case) {
    cflags |= REG_ICASE;
  }

  if (regcomp(&regex, options->patterns[0], cflags) != 0) {
    if (!options->no_messages) {
      fprintf(stderr, "Invalid regular expression\n");
    }
    return;
  }

  while (res && fgets(line, sizeof(line), file)) {
    line_number++;
    int match = regexec(&regex, line, 0, NULL, 0) == 0;

    if ((match && !options->invert_match) ||
        (!match && options->invert_match)) {
      match_count++;

      if (options->files_with_matches) {
        printf("%s\n", filename);
        res = 0;
      }

      if (res && !options->count) {
        if (!options->no_filenames && options->file_count > 1) {
          printf("%s:", filename);
        }

        if (options->line_numbers) {
          printf("%d:", line_number);
        }

        if (options->only_matching) {
          regmatch_t pmatch;
          char *ptr = line;
          int found;
          do {
            found = regexec(&regex, ptr, 1, &pmatch, 0) == 0;
            if (found) {
              if (!options->no_filenames && options->file_count > 1) {
                printf("%s:", filename);
              }
              if (options->line_numbers) {
                printf("%d:", line_number);
              }
              printf("%.*s\n", (int)(pmatch.rm_eo - pmatch.rm_so),
                     ptr + pmatch.rm_so);
              ptr += pmatch.rm_eo;
            }
          } while (found);
        } else {
          printf("%s", line);
          if (line[strlen(line) - 1] != '\n') {
            printf("\n");
          }
        }
      }
    }
  }

  if (res && options->count) {
    if (!options->no_filenames && options->file_count > 1) {
      printf("%s:", filename);
    }
    printf("%d\n", match_count);
  }

  regfree(&regex);
}

int main(int argc, char *argv[]) {
  int res = 0;
  GrepOptions options = {0};
  FILE *file = NULL;
  int pattern_valid = 1;

  parse_options(argc, argv, &options);
  options.file_count = argc - optind;

  if (options.pattern_count == 0 && optind < argc) {
    if (options.pattern_count >= MAX_PATTERNS) {
      fprintf(stderr, "Too many patterns\n");
      res = 1;
      pattern_valid = 0;
    } else {
      strncpy(options.patterns[options.pattern_count++], argv[optind++],
              PATTERN_SIZE - 1);
      options.patterns[options.pattern_count - 1][PATTERN_SIZE - 1] = '\0';
      options.file_count--;
    }
  }
  if (pattern_valid) {
    if (options.pattern_count == 0) {
      fprintf(stderr, "No pattern provided\n");
      res = 1;
    } else if (optind >= argc) {
      process_file(stdin, &options, NULL);
      res = 1;
    } else {
      int i = optind;
      while (i < argc && res == 0) {
        file = fopen(argv[i], "r");
        if (file != NULL) {
          process_file(file, &options, options.no_filenames ? NULL : argv[i]);
          fclose(file);
          file = NULL;
        } else if (!options.no_messages) {
          fprintf(stderr, "%s: %s\n", argv[i], strerror(errno));
        }
        i++;
      }
      res = 1;
    }
  }
  if (file != NULL) {
    fclose(file);
  }

  return res;
}