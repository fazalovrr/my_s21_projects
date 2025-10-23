#include "file_processor.h"

bool open_file(const char *filename, FILE **file) {
  bool res = true;
  *file = fopen(filename, "r");
  if (*file == NULL) {
    print_error("Cannot open file");
    res = false;
  }
  return res;
}

void process_file(FILE *file, const FileConfig *config) {
  char buffer[BUFFER_SIZE];

  while (fgets(buffer, sizeof(buffer), file)) {
    if (config->type == PROCESS_CAT) {
    } else if (config->type == PROCESS_GREP) {
    }
  }
}

void process_files(int argc, char *argv[], const FileConfig *config) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-') {
      FILE *file;
      if (open_file(argv[i], &file)) {
        process_file(file, config);
        fclose(file);
      }
    }
  }
}