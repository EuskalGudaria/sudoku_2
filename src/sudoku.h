#ifndef SUDOKU_H
#define SUDOKU_H

#define _GNU_SOURCE

#define VERSION 1
#define SUBVERSION 0
#define REVISION 0

#define MAX_FILES 10

#include <stdbool.h>
#include <stdio.h>

typedef struct {
  size_t size;
  char **cells;
} grid_t;

static grid_t *grid_alloc(size_t size);
static void grid_free(grid_t *grid);
static void grid_print(const grid_t *grid, FILE *fd);
static bool check_char(const grid_t *grid, const char c);
static grid_t *file_parser(char *filename);

#endif /* SUDOKU_H */