CC=clang

# use for windows binary compile
WCC=i686-w64-mingw32-gcc

all: BlackMura.o src/main.c
	$(CC) -o bin/BlackMura BlackMura.o src/main.c

BlackMura.o: src/BlackMura.h src/BlackMura.c
	$(CC) -c src/BlackMura.c -o BlackMura.o

debug: BlackMura_dbg.o src/main.c
	$(CC) -g -o bin/BlackMura_dbg BlackMura_dbg.o src/main.c

BlackMura_dbg.o: src/BlackMura.h src/BlackMura.h
	$(CC) -g -c src/BlackMura.c -o BlackMura_dbg.o

test: src/test_input.c
	$(CC) -o bin/test_input src/test_input.c

testWindows: src/test_input.c
	$(WCC) -o bin/test_input.exe src/test_input.c

clean:
	rm -rf bin/*
