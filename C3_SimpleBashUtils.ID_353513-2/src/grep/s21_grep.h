#include <stdio.h>

typedef int bool;
#define true 1
#define false 0

#define MAX_PATTERNS 100
#define PATTERN_SIZE 1024
#define LINE_SIZE 4096

typedef struct {
  bool ignore_case;
  bool invert_match;
  bool count;
  bool files_with_matches;
  bool line_numbers;
  bool no_filenames;
  bool no_messages;
  bool only_matching;
  bool use_extended_regexp;
  char patterns[MAX_PATTERNS][PATTERN_SIZE];
  int pattern_count;
  int file_count;
} GrepOptions;

bool parse_options(int argc, char *argv[], GrepOptions *options);
bool read_patterns_from_file(const char *filename, GrepOptions *options);
void process_file(FILE *file, GrepOptions *options, const char *filename);