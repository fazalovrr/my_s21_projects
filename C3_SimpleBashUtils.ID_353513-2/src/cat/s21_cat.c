#include "s21_cat.h"

#include <getopt.h>
#include <stdlib.h>
#include <string.h>

int parse_options(int argc, char *argv[], CatOptions *options) {
  int opt;
  int error_flag = 0;

  while ((opt = getopt(argc, argv, "beEnstTv")) != -1 && !error_flag) {
    switch (opt) {
      case 'b':
        options->number_nonblank = true;
        options->number = false;
        break;
      case 'e':
        options->show_ends = true;
        options->show_nonprinting = true;
        break;
      case 'E':
        options->show_ends = true;
        break;
      case 'n':
        if (!options->number_nonblank) {
          options->number = true;
        }
        break;
      case 's':
        options->squeeze_blank = true;
        break;
      case 't':
        options->show_tabs = true;
        options->show_nonprinting = true;
        break;
      case 'T':
        options->show_tabs = true;
        break;
      case 'v':
        options->show_nonprinting = true;
        break;
      default:
        print_error("Invalid option");
        error_flag = 1;
    }
  }

  return error_flag;
}

int process_line(char *buffer, size_t len, CatOptions *options,
                 int *line_number) {
  int result = 0;
  bool is_empty = (len == 0 || buffer[0] == '\n');

  if (options->number_nonblank && !is_empty) {
    if (printf("%6d\t", (*line_number)++) < 0) result = 1;
  } else if (options->number) {
    if (printf("%6d\t", (*line_number)++) < 0) result = 1;
  }

  for (size_t i = 0; i < len && !result; i++) {
    unsigned char c = buffer[i];

    if (options->show_ends && c == '\n') {
      if (putchar('$') == EOF) result = 1;
    }

    if (options->show_tabs && c == '\t') {
      if (printf("^I") < 0) result = 1;
    } else if (options->show_nonprinting && (c < 32 || c == 127)) {
      print_special_char(c);
    } else {
      if (putchar(c) == EOF) result = 1;
    }
  }

  return result;
}

int process_cat_file(FILE *file, CatOptions *options, int *line_number) {
  char buffer[BUFFER_SIZE];
  bool last_was_empty = false;
  int result = 0;

  while (fgets(buffer, sizeof(buffer), file) != NULL && !result) {
    size_t len = strlen(buffer);
    bool is_empty = (len == 0 || buffer[0] == '\n');
    bool should_process = true;

    if (options->squeeze_blank) {
      should_process = !(is_empty && last_was_empty);
      last_was_empty = is_empty;
    }

    if (should_process) {
      result = process_line(buffer, len, options, line_number);
    }
  }

  if (ferror(file)) result = 1;
  return result;
}

int process_cat_files(int argc, char *argv[], CatOptions *options) {
  int result = 0;
  int i = optind;
  int line_number = 1;

  while (i < argc && !result) {
    if (argv[i][0] == '-' && argv[i][1] != '\0') {
      print_error("Invalid option or filename");
      result = 1;
    } else {
      FILE *file = fopen(argv[i], "r");
      if (!file) {
        print_file_error(argv[i]);
        result = 1;
      } else {
        int file_result = process_cat_file(file, options, &line_number);
        if (fclose(file)) file_result = 1;
        if (file_result) result = 1;
      }
    }
    i++;
  }

  return result;
}

int main(int argc, char *argv[]) {
  CatOptions options = {false};
  int result = 0;

  if (parse_options(argc, argv, &options)) {
    result = 1;
  } else if (optind >= argc) {
    print_error("No input files specified");
    result = 1;
  } else {
    result = process_cat_files(argc, argv, &options);
  }

  return result;
}