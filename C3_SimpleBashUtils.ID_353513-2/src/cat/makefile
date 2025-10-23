# Упрощенный Makefile для s21_cat

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
SRC = s21_cat.c ../common/file_processor.c ../common/output.c
OBJ = $(SRC:.c=.o)
TARGET = s21_cat

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

rebuild: clean all

memcheck:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./s21_cat s21_cat.c

.PHONY: all clean rebuild