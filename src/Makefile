CC=gcc
CPPFLAGS=
CFLAGS=
LDFLAGS=
OUTPUT=sudoku
.PHONY: all

all:
	${CC} ${CFLAGS} -o ${OUTPUT} src/*.c
clean:
	rm -f ${OUTPUT} *.o
help:
	@echo "all: Run the whole build of sudoku;"
	@echo "clean: Remove all files produced by the compilation;"
	@echo "help: Display the main targets of this Makefile with a short description."
