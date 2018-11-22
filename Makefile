CC=clang -Wall -DLINUX

# use for windows binary compile
WCC=i686-w64-mingw32-gcc --static -DWINDOWS

all: tool.o BlackMura.o src/main.c
	$(CC) -o bin/BlackMura tool.o BlackMura.o src/main.c

BlackMura.o: src/BlackMura.h src/BlackMura.c
	$(CC) -c src/BlackMura.c -o BlackMura.o

tool.o: src/tool.h src/tool.c
	$(CC) -c src/tool.c -o tool.o

debug: BlackMura_dbg.o src/main.c
	$(CC) -g -o bin/BlackMura_dbg BlackMura_dbg.o src/main.c

BlackMura_dbg.o: src/BlackMura.h src/BlackMura.h
	$(CC) -g -c src/BlackMura.c -o BlackMura_dbg.o

Windows: BlackMura_win.o tool_win.o src/main.c
	$(WCC) -o bin/BlackMura_win.exe tool_win.o BlackMura_win.o src/main.c

BlackMura_win.o: src/BlackMura.h src/BlackMura.c
	$(WCC) -c src/BlackMura.c -o BlackMura_win.o

tool_win.o: src/tool.h src/tool.o
	$(WCC) -c src/tool.c -o tool_win.o

test: src/test_input.c
	$(CC) -o bin/test_input src/test_input.c

testWindows: src/test_input.c
	$(WCC) -o bin/test_input.exe src/test_input.c

clean:
	rm -rf bin/* *.o
