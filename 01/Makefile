CC      := clang
CFLAGS  := -std=c11 -Wall -Wextra -Werror -g

BUILD   := build
SRCS    := $(wildcard *.c)
OBJS    := $(SRCS:%.c=$(BUILD)/%.o)

TEST_BIN := $(BUILD)/test

.PHONY: all test clean

all: $(OBJS)

# Link every object (chips + test.c) into one program and run it.
test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD)/%.o: %.c $(wildcard *.h) | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $@

clean:
	rm -rf $(BUILD)
