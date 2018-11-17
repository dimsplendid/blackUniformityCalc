CC=clang

# use for windows binary compile
WCC=i686-w64-mingw32-gcc

all: src/test_input.c
	$(CC) -o bin/test_input src/test_input.c

Windows: src/test_input.c
	$(WCC) -o bin/test_input.exe src/test_input.c

clean:
	rm -rf bin/*
