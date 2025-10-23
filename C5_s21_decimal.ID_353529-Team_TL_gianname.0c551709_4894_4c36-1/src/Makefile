CC = gcc
AR = ar
RM = rm -rf
CFLAGS = -Wall -Werror -Wextra -std=c11
AR_FLAGS = rcs
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
LIB_NAME = s21_decimal.a
LIB_NAME_GCOV = s21_decimal_gcov.a
EXE_NAME = tests_decimal
BUILD_DIR := build_lib
TEST_BUILD_DIR := build_test
LIB_SRCS = $(wildcard functions/*.c)
EXE_SRC = $(wildcard tests/*.c)
LIB_OBJS = $(LIB_SRCS:functions/%.c=$(BUILD_DIR)/%.o)
LIB_OBJS_GCOV = $(LIB_SRCS:functions/%.c=$(TEST_BUILD_DIR)/%_gcov.o)
EXE_OBJS = $(EXE_SRC:%.c=%.o)
UNAME_S := $(shell uname -s)
# Coverage files
COVERAGE_NAME = coverage.info
COVERAGE_DIR = coverage_report
ALL_OBJS_GCNO = $(patsubst %.c,%_gcov.gcno,$(LIB_SRCS) $(EXE_SRC))
ALL_OBJS_GCDA = $(patsubst %.c,%_gcov.gcda,$(LIB_SRCS) $(EXE_SRC))

ifeq ($(UNAME_S),Linux)
	OPEN_CMD = xdg-open
	CHFLAGS = -lcheck -L/usr/lib/x86_64-linux-gnu/ -lsubunit -lm -lrt -lpthread -D_GNU_SOURCE
endif
ifeq ($(UNAME_S),Darwin)
	OPEN_CMD = open
	CHFLAGS = -lcheck
endif

all: $(LIB_NAME)

$(COVERAGE_DIR):
	mkdir -p $(COVERAGE_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: functions/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_NAME): $(LIB_OBJS)
	$(AR) $(AR_FLAGS) $@ $^

$(TEST_BUILD_DIR):
	mkdir -p $(TEST_BUILD_DIR)

$(TEST_BUILD_DIR)/%_gcov.o: functions/%.c  | $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c $< -o $@

$(LIB_NAME_GCOV): $(LIB_OBJS_GCOV)
	$(AR) $(AR_FLAGS) $@ $^

$(EXE_NAME): $(EXE_OBJS) $(LIB_NAME_GCOV)
	$(CC) $(CFLAGS) $(GCOV_FLAGS)  $^ -o $@ $(CHFLAGS) -w 

test: $(EXE_NAME)

gcov_report: $(EXE_NAME) $(COVERAGE_DIR)
	./$(EXE_NAME)
	gcovr --html-details -o $(COVERAGE_DIR)/report.html
	$(OPEN_CMD) $(COVERAGE_DIR)/report.html > /dev/null 2>&1 &

lcov_report: $(EXE_NAME) $(COVERAGE_DIR)
	./$(EXE_NAME)
	lcov --capture --directory . --output-file $(COVERAGE_NAME)  --rc lcov_branch_coverage=1
	genhtml $(COVERAGE_NAME) --output-directory $(COVERAGE_DIR)
	$(OPEN_CMD) $(COVERAGE_DIR)/index.html > /dev/null 2>&1 &

clean:
	$(RM) $(LIB_OBJS) $(LIB_NAME) $(BUILD_DIR)
	$(RM) $(LIB_OBJS_GCOV) $(LIB_NAME_GCOV) $(EXE_OBJS) $(EXE_NAME) $(TEST_BUILD_DIR)
	$(RM) $(ALL_OBJS_GCNO) $(ALL_OBJS_GCDA) $(COVERAGE_NAME) $(COVERAGE_DIR)
	$(RM) /test/*.o
	

rebuild: clean all

.PHONY: all test clean rebuild gcov_report open_report