#include "colors.h"
#include <stdio.h>
#include <stdlib.h>

#include "grid.h"


/* Internal structure (hiden from outside) to represent a sudoku grid */
#include <err.h>


struct _grid_t
{
	size_t size;
	colors_t **cells;
};

grid_t *grid_copy(const grid_t *grid)
{
	if(grid == NULL)
	{
		return NULL;
	}
	grid_t *new_grid = grid_alloc(grid->size);
	//nouveau
	if(new_grid == NULL)
	{
		warnx(EXIT_FAILURE, "Bad size of grid");
		return NULL;
	}

	
	for(size_t i =0 ;i < new_grid->size; i++)
	{
		for(size_t j =0 ;j < new_grid->size; j++)
		{
			new_grid->cells[i][j] = grid->cells[i][j];
		}
	}
	return new_grid;
		 
}

char *grid_get_cell(const grid_t *grid, const size_t row, const size_t column) {
  if (grid == NULL) {
    return NULL;
  }

  size_t size = grid->size;
  if (row > size || column > size) {
    return NULL;
  }

  int string_index = 0;
  colors_t color_cell = grid->cells[row][column];
  char *color_string = malloc(colors_count(color_cell) + 1 * sizeof(char));
  for (size_t color_id = 0; color_id < MAX_COLORS; color_id++) {
    if (colors_is_in(color_cell, color_id)) {
      color_string[string_index] = color_table[color_id];
      string_index++;
    }
  }
  color_string[string_index] = '\0';
  return color_string;
}

size_t grid_get_size(const grid_t *grid)
{	
	if(grid == NULL)
	{
		return 0;
	}
	return grid->size;
}

void grid_set_cell(grid_t *grid, const size_t row, const size_t column, const char color)
{		
				
	if (grid == NULL)
	{
		warnx("Bad data in grid_set_cell (grid == NULL)");
		return 0;
	}
	if (row > grid->size)
	{
		warnx("Bad data in grid_set_cell (row > grid->size)");
		return 0;
	}
	if (column > grid->size) 
	{
		warnx("Bad data in grid_set_cell (column > grid->size)");
		return 0;
	}
	if (!grid_check_char(grid, color))
	{
		warnx("Bad data in grid_set_cell (grid_check_c(c))");
		return 0;
	}
	if( color == '_')
	{
		grid->cells[row][column] = colors_full(grid->size);
		return;
	}

  char *index = strchr(color_table, color);
  size_t color_id = (index - color_table) / sizeof(char);
  grid->cells[row][column] = colors_set(color_id);
}

static bool grid_check_c(char c)
{
	return((c >= '1' && c <= '9') || (c == '_') || (c >= 'A' && c <= 'Z') || (c == '@') || (c >= 'a' && c <='z') || (c == '&') || (c == '*'));
}

bool grid_check_size(size_t size)
{
  return (size == 1 || size == 4 || size == 9 || size == 16 || size == 25 || size == 36 || size == 49 || size == 64);
}

/*
void grid_set_cell(grid_t *grid, const size_t row, const size_t column, const char c)
{					
	if (grid == NULL)
	{
		errx(EXIT_FAILURE, "Bad data in grid_set_cell (grid == NULL)");
	}
	if (row > grid->size)
	{
		errx(EXIT_FAILURE, "Bad data in grid_set_cell (row > grid->size)");
	}
	if (column > grid->size) 
	{
		errx(EXIT_FAILURE, "Bad data in grid_set_cell (column > grid->size)");
	}
	if (!grid_check_c(c))
	{
		errx(EXIT_FAILURE, "Bad data in grid_set_cell (grid_check_c(c))");
	}

	grid->cells[row][column] = c;
	
}
*/
bool grid_check_char (const grid_t *grid, const char c)
{	
	size_t size = grid->size;
	
	switch (size)
	{
	case 1:
		return( (c == '1') || (c == '_'));
	case 4:
		return((c >= '1' && c <= '4' ) || (c == '_'));
	case 9:
		return((c >= '1' && c <= '9') || (c == '_'));
	case 16:
		return((c >= '1' && c <= '9') || (c == '_') || (c >= 'A' && c <= 'G'));
	case 25:
		return((c >= '1' && c <= '9') || (c == '_') || (c >= 'A' && c <= 'P'));
	case 36:
		return((c >= '1' && c <= '9') || (c == '_') || (c >= 'A' && c <= 'Z') || (c == '@'));
	case 49:
		return((c >= '1' && c <= '9') || (c == '_') || (c >= 'A' && c <= 'Z') || (c == '@') || (c >= 'a' && c <= 'm'));
	case 64:
		return((c >= '1' && c <= '9') || (c == '_') || (c >= 'A' && c <= 'Z') || (c == '@') || (c >= 'a' && c <= 'z') || (c == '&') || (c == '*'));
	default:
	 	errx(EXIT_FAILURE, "error bad size of grid %ld",size);
	}
}

grid_t *grid_alloc (size_t size)
{
	if (!grid_check_size(size))
	{
		warnx("Bad size of grid");
		return NULL;
	}
	
	colors_t **cells = malloc(size*sizeof(colors_t*));
	if (cells == NULL)
	{
		errx(EXIT_FAILURE, "calloc is impossible (first)");
	}
	
	for (size_t i = 0; i< size; i++)
	{
		cells[i] = calloc(size, sizeof(colors_t));
		
		if (cells[i] == NULL)
		{
			errx(EXIT_FAILURE, "calloc is impossible (second)");
		}
	}
		
	grid_t *grid = malloc(sizeof(grid_t));
	if (grid == NULL)
	{
		errx(EXIT_FAILURE, "malloc of grid fail");
	}
	grid->size = size;
	grid->cells = cells;
	grid_t *grid_ptr = grid;
	return grid_ptr;
}

void grid_free (grid_t *grid)
{

	if (grid == NULL)
	{
		warnx("error grid is already free");
		return;
	}

	size_t size = grid->size;
	colors_t **cells = grid->cells;
	for (size_t i = 0; i < size; i++)
	{
		free(cells[i]);	
	}
	free(cells);
	free(grid);
}

void grid_print (const grid_t *grid, FILE *fd)
{
	if (grid == NULL)
	{
		warnx("error grid is NULL");
		return;
	}
	if (fd == NULL)
	{
		warnx("error FILE is NULL in grid_print");
		return;
	}
	
	size_t size = grid -> size;
	colors_t **cells = grid -> cells;
	
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			fputc(cells[i][j], fd);
			fputc(' ', fd);
		}
		fputc('\n', fd);
	}
}
