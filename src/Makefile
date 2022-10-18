CPPFLAGS= -I../include -DDEBUG
CFLAGS= -std=c11 -Wall -Wextra -g -O2 -pedantic
LDFLAGS= -lm
OUTPUT = sudoku
.PHONY: all clean 

all: sudoku.o colors.o
	${CC} ${CFLAGS} ${CPPFLAGS} -o ${OUTPUT} sudoku.o colors.o ${LDFLAGS}

colors.o: colors.c
	${CC} ${CFLAGS} ${CPPFLAGS} -c colors.c
sudoku.o: sudoku.c
	${CC} ${CFLAGS} ${CPPFLAGS} -c sudoku.c 

clean:
	rm -f ${OUTPUT} *.o

help:
	@echo "all: 	build and execute the whole project"
	@echo "clean: 	delete all object files"
	@echo "help: Display the main targets of this Makefile with a short description"