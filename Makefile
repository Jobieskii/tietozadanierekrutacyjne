CC := clang 
CFLAGS := --std=c11 -Wall -Wextra -Wno-missing-braces -lpthread

BUILD_DIR := ./build
SRC_DIR := ./src

SRCS := $(shell find $(SRC_DIR) -name '*.c')

all:
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/proc_stat $(SRCS)


TEST_DIR := ./build/test
TEST_SRC_DIR := ./tests

test:
	mkdir -p $(TEST_DIR)
	$(CC) $(CFLAGS) -o $(TEST_DIR)/file_func $(TEST_SRC_DIR)/file_func_test.c $(SRC_DIR)/file_func.c
	$(TEST_DIR)/file_func
	rm -r $(TEST_DIR)
