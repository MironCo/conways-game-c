CC = clang
TARGET = game
CFLAGS = -Wall -std=c99 -Iinclude -Ivendor/raylib/src
LDFLAGS = -framework CoreVideo -framework IOKit -framework Cocoa -framework OpenGL vendor/raylib/src/libraylib.a

# Build raylib first, then your project
build: vendor/raylib/src/libraylib.a src/main.c
	$(CC) $(CFLAGS) src/*.c -o $(TARGET) $(LDFLAGS)

# Build raylib library
vendor/raylib/src/libraylib.a:
	cd vendor/raylib/src && make

run: build
	./$(TARGET)

clean: 
	rm -f $(TARGET)

clean-all: clean
	cd vendor/raylib/src && make clean

.PHONY: build run clean clean-all