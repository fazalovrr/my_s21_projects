#include <stdio.h>
#include <string.h>

#include "output.h"

typedef int bool;
#define true 1
#define false 0

#define BUFFER_SIZE 4096
#define MAX_FILES 100

typedef enum { PROCESS_CAT, PROCESS_GREP } FileProcessorType;

typedef struct {
  FileProcessorType type;
  union {
    struct {
      bool number_nonblank;
      bool show_ends;
      bool number;
      bool squeeze_blank;
      bool show_tabs;
      bool show_nonprinting;
    } cat_options;
    struct {
      bool ignore_case;
      bool invert_match;
      bool count_lines;
      bool print_filename;
    } grep_options;
  };
} FileConfig;

bool open_file(const char *filename, FILE **file);
void process_file(FILE *file, const FileConfig *config);
void process_files(int argc, char *argv[], const FileConfig *config);
