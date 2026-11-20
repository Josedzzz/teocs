CC      := clang
CFLAGS  := -std=c11 -Wall -Wextra -Werror -g

# Project 01 chips are reused as parts: its headers and sources come from ../01
CHIPS01 := ../01
vpath %.c $(CHIPS01)

BUILD   := build
SRCS    := $(wildcard *.c) nand.c gates.c
OBJS    := $(SRCS:%.c=$(BUILD)/%.o)
HDRS    := $(wildcard *.h) $(wildcard $(CHIPS01)/*.h)

TEST_BIN := $(BUILD)/test

.PHONY: all test clean

all: $(OBJS)

# Link every object (project 01 chips + this project + test.c) and run it.
test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD)/%.o: %.c $(HDRS) | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $@

clean:
	rm -rf $(BUILD)
