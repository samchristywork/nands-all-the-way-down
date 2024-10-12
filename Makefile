CC=gcc
CFLAGS=-Wall -Wpedantic -Wextra -g
LIBS=-lSDL2 -lSDL2_ttf -lSDL2_gfx -lm

all: build/main

.PHONY: objects
objects: $(patsubst src/%.c, build/%.o, $(wildcard src/*.c))

build/%.o: src/%.c
	mkdir -p build
	$(CC) -c $(CFLAGS) $< -o $@

build/main: objects
	${CC} build/*.o ${LIBS} -o $@

.PHONY: run
run: all
	./build/main

.PHONY: clean
clean:
	rm -rf build
