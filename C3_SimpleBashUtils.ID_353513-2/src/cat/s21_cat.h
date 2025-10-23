#include <stdio.h>

#define BUFFER_SIZE 4096

typedef int bool;
#define true 1
#define false 0

typedef struct {
  bool number_nonblank;
  bool show_ends;
  bool number;
  bool squeeze_blank;
  bool show_tabs;
  bool show_nonprinting;
} CatOptions;

void print_special_char(unsigned char c);
void print_file_error(const char *filename);
void print_error(const char *message);