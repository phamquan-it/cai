CC = gcc
CFLAGS = -Wall -Wextra -O2 -I./src
LDFLAGS = -lm
TARGET = bin/cai
SRCS = src/main.c src/brain.c src/markov.c src/utils.c src/context.c \
       src/intent.c src/attribute.c src/definition.c src/physics.c \
       src/coder.c src/knowledge.c src/chemistry.c src/vietnamese.c \
       src/bridge.c
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

.PHONY: all clean run
