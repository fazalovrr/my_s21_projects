CC = gcc
AR = ar
RM = rm -rf
CFLAGS = -Wall -Werror -Wextra -std=c11
GCOV_FLAGS = -fprofile-arcs -ftest-coverage -g
AR_FLAGS = rcs

SRC_DIR = func
TEST_FILE = tests/test.c
LIB_NAME = s21_matrix.a
COVERAGE_DIR = coverage_report
REPORT_HTML = $(COVERAGE_DIR)/report.html

LIB_SRCS = $(wildcard $(SRC_DIR)/*.c)
LIB_OBJS = $(LIB_SRCS:.c=.o)
LIB_OBJS_GCOV = $(LIB_SRCS:.c=.gcov.o)

.PHONY: all clean test gcov_report

all: $(LIB_NAME)

$(LIB_NAME): $(LIB_OBJS)
	$(AR) $(AR_FLAGS) $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o func/*.o test/*.o *.gcno *.gcda $(LIB_NAME) test_exec $(REPORT_HTML) $(COVERAGE_DIR) unit_test

test: $(LIB_NAME)
	$(CC) $(CFLAGS) $(TEST_FILE) $(LIB_NAME) -o test_exec -lcheck -lm -pthread -lrt -lsubunit
	./test_exec

gcov_report: clean
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c $(LIB_SRCS)
	$(AR) $(AR_FLAGS) $(LIB_NAME) *.o
	$(CC) $(CFLAGS) $(GCOV_FLAGS) tests/test.c $(LIB_NAME) -o test_exec -lcheck -lm -pthread -lrt -lsubunit
	./test_exec
	mkdir -p $(COVERAGE_DIR)
	gcovr --html-details -o $(REPORT_HTML) --exclude tests/test.c
	xdg-open $(REPORT_HTML) > /dev/null 2>&1
