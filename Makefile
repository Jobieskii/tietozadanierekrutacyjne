CC := gcc
CFLAGS := --std=c11 -Wall -Wextra -Wno-missing-braces

BUILD_DIR := ./build
SRC_DIR := ./src

SRCS := $(shell find $(SRC_DIR) -name '*.c')

all:
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/proc_stat $(SRCS)
