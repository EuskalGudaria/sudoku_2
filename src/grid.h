#ifndef GRID_H
#define GRID_h

#define MAX_GRID_SIZE 64
#define EMPTY_CELL '_'
#include <stdbool.h>
#include <stdio.h> 

static const char color_table[] =
	"123456789" "ABCDEFGHUJKLMNOPQRSTUVWXYZ" "@" "abcdefghijklmnopqrstuvwxyz" "&*";
/* Sudoku grid (forward declarationn to hide the implementation) */
typedef struct _grid_t grid_t;

grid_t *grid_copy(const grid_t *grid);

char *grid_get_cell(const grid_t *grid, const size_t row, const size_t column);

size_t grid_get_size(const grid_t *grid);

void grid_set_cell(grid_t *grid, const size_t row, const size_t column, const char color);

static bool grid_check_c(char c);

bool grid_check_size(size_t size);

bool grid_check_char (const grid_t *grid, const char c);

grid_t *grid_alloc (size_t size);

void grid_free (grid_t *grid);

void grid_print (const grid_t *grid, FILE *fd);

#endif /* GRID_H */
