CC      := clang
CFLAGS  := -std=c11 -Wall -Wextra -Werror -g

BUILD   := build
SRCS    := $(wildcard *.c)
OBJS    := $(SRCS:%.c=$(BUILD)/%.o)

.PHONY: all clean

all: $(OBJS)

$(BUILD)/%.o: %.c $(wildcard *.h) | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $@

clean:
	rm -rf $(BUILD)
