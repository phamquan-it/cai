CC = gcc
CFLAGS = -Wall -Wextra -O2 -I./src
LDFLAGS = -lm
TARGET = bin/cai
SRCS = src/main.c src/brain.c src/markov.c src/utils.c
OBJS = $(SRCS:src/%.c=build/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p bin
	$(CC) -o $@ $^ $(LDFLAGS)
	@echo "✅ Build complete! Run ./$(TARGET)"

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build bin
	@echo "🧹 Cleaned"

run: $(TARGET)
	./$(TARGET)

debug: CFLAGS += -g -DDEBUG
debug: clean $(TARGET)
	gdb ./$(TARGET)

help:
	@echo "Commands:"
	@echo "  make        Build project"
	@echo "  make clean  Clean build files"
	@echo "  make run    Build and run"
	@echo "  make debug  Build with debug symbols"

.PHONY: all clean run debug help
