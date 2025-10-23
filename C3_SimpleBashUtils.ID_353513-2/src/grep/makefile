CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
TARGET = s21_grep
SOURCES = s21_grep.c ../common/output.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

rebuild: clean all

memcheck:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./s21_grep "pattern" s21_grep.c s21_grep.c