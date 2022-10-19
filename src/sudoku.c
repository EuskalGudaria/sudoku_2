#include "sudoku.h"
#include "colors.h"
#include "grid.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <err.h>
#include <getopt.h>

#define MAX_GRID_SIZE 64

static bool verbose = false;
/*
typedef struct
{
        size_t size;
        char **cells;
}grid_t;
*/
static bool check_size(size_t size) {
  return (size == 1 || size == 4 || size == 9 || size == 16 || size == 25 ||
          size == 36 || size == 49 || size == 64);
}

static bool check_c(char c) {
  return ((c >= '1' && c <= '9') || (c == '_') || (c >= 'A' && c <= 'Z') ||
          (c == '@') || (c >= 'a' && c <= 'z') || (c == '&') || (c == '*'));
}
/*MOI
void grid_set_cell(grid_t *grid, const size_t row, const size_t column, const
char c)
{
        if (grid == NULL)
        {
                errx(EXIT_FAILURE, "Bad data in grid_set_cell (grid == NULL)");
        }
        if (row > grid->size)
        {
                errx(EXIT_FAILURE, "Bad data in grid_set_cell (row >
grid->size)");
        }
        if (column > grid->size)
        {
                errx(EXIT_FAILURE, "Bad data in grid_set_cell (column >
grid->size)");
        }
        if (!check_c(c))
        {
                errx(EXIT_FAILURE, "Bad data in grid_set_cell (check_c(c))");
        }

        grid->cells[row][column] = c;

}
*/
/*
static grid_t *grid_alloc (size_t size)
{
        if (!check_size(size))
        {
                errx(EXIT_FAILURE, "Bad size of grid");
        }

        char **cells = malloc(size*sizeof(char*));
        if (cells == NULL)
        {
                errx(EXIT_FAILURE, "calloc is impossible (first)");
        }

        for (size_t i = 0; i< size; i++)
        {
                cells[i] = calloc(size, sizeof(char));

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
*//*
static void grid_free (grid_t *grid)
{
	if (grid == NULL)
	{
		errx(EXIT_FAILURE, "error grid is already free");
	}
	
	size_t size = grid->size;
	char **cells = grid->cells;
	for (size_t i = 0; i < size; i++)
	{
		free(cells[i]);	
	}
	free(cells);
	free(grid);
}
*//*

static void grid_print (const grid_t *grid, FILE *fd)
{
	if (grid == NULL)
	{
		errx(EXIT_FAILURE, "error grid is NULL");
	}
	if (fd == NULL)
	{
		errx(EXIT_FAILURE, "error FILE is NULL in grid_print");
	}
	
	size_t size = grid -> size;
	char **cells = grid -> cells;
	
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


*//*
static bool check_char (const grid_t *grid, const char c)
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
	 	errx(EXIT_FAILURE, "error bad size of grid");
	}
}
*/
static grid_t *file_parser(char *filename) {
  char first_row[MAX_GRID_SIZE];
  struct grid_t *grid;
  FILE *file;
  file = fopen(filename, "r+");
  int returnCode;
  if (file == NULL) {
    warnx("error FILE is NULL in file_parser %s", filename);
    return NULL;
  }

  int row_size = 0;
  int row = 0;
  int size;
  char c;
  char last_c;
  bool empty = true;
  bool last_char_valid = false;

  while ((c = fgetc(file)) != EOF) {
    switch (c) {
    case ' ':
    case '\t':
      last_char_valid = false;
      break;
    case '#':

      while ((c = fgetc(file)) != '\n' && c != EOF) {
        continue;
      }
      last_char_valid = false;
      break;

    case '\n':

      if (row_size == 0) {
        break;
      } else {
        if (row == 0) {
          if (!check_size(row_size)) {
            warnx("Bad size of grid");
            return NULL;
          }
          grid = grid_alloc(row_size);
          // Nouveau
          if (grid == NULL) {
            warnx(EXIT_FAILURE, "Bad size of grid");
            return NULL;
          }
          size = row_size;
        }
        if (row_size != size) {
          warnx("error all must be at the same dimension");
          return NULL;
        }
        if (row == size) {
          warnx("more column than row");
          return NULL;
        }
        for (int column = 0; column < row_size; column++) {
          if (grid_check_char(grid, first_row[column])) {
            grid_set_cell(grid, row, column, first_row[column]);
          } else {
            warnx("unauthorised char");
            return NULL;
          }
        }
        empty = false;
        row_size = 0;
        row++;
        last_char_valid = false;
      }
      break;

    default:
      if (check_c(c)) {
        first_row[row_size] = c;
        last_char_valid = true;
      } else {
        errx(EXIT_FAILURE, "Bad char");
      }
      row_size++;
    }
    last_c = c;
  }
  if (last_c != '\n') {
    if (row_size == 0 && empty) {
      errx(EXIT_FAILURE, "a grid is empty");
    } else if (last_char_valid) {
      if (row == 0) {
        if (!check_size(row_size)) {
          warnx("Bad size of grid");
          return NULL;
        }
        grid = grid_alloc(row_size);
        // Nouveau
        if (grid == NULL) {
          warnx(EXIT_FAILURE, "Bad size of grid");
          return NULL;
        }
        size = row_size;
      }
      if (row_size != size) {
        warnx("error all must be at the same dimension");
        return NULL;
      }
      if (row == size) {
        warnx("more column than row");
        return NULL;
      }
      for (int column = 0; column < row_size; column++) {
        if (grid_check_char(grid, first_row[column])) {
          grid_set_cell(grid, row, column, first_row[column]);
        } else {
          warnx("unauthorised char");
          return NULL;
        }
      }
      row_size = 0;
      row++;
    }
  }
  if (row > 0) {
    if (row < size) {
      errx(EXIT_FAILURE, "number of row is different of the number of column");
    }

    return grid;
  }
  warnx("A grid has no char");
  return NULL;

  returnCode = fclose(file);
  if (returnCode == EOF) {
    warnx(stderr, "File not close");
  }
}

int main(int argc, char *argv[]) {

  int optc;
  bool all = false;
  bool unique = false;
  bool generator = false;
  int size = 9;
  bool exit_failure = false;
  bool version = false;
  bool help = false;
  int returnCode;

  FILE *output_file = stdout;

  const struct option long_opts[] = {{"help", no_argument, NULL, 'h'},
                                     {"version", no_argument, NULL, 'V'},
                                     {"verbose", no_argument, NULL, 'v'},
                                     {"output", required_argument, NULL, 'o'},
                                     {"all", no_argument, NULL, 'a'},
                                     {"unique", no_argument, NULL, 'u'},
                                     //{ "generate", no_argument, NULL, 'g' },
                                     {"generate", optional_argument, NULL, 'g'},
                                     {NULL, 0, NULL, 0}};

  while ((optc = getopt_long(argc, argv, "g::auhVvo:", long_opts, NULL)) !=
         -1) {
    switch (optc) {
    case 'h':
      printf("Usage: sudoku[-a|-o FILE|-v|-V|-h] FILE ...\n"
             "       sudoKu -g[SIZE] [-u|-o FILE|-v|-V|-h]\n"
             "Solve or generatee Sudoku gride of various sizes "
             "(1,4,9,16,25,36,49,65)\n\n"

             "-a,--all search for all possible solution\n"
             "-g[N},--generate[-N] generate a grid of size NxN (default:9)\n"
             "-u,--unique generate a grid with unique solution\n"
             "-o FILE,--output FILE write solution to SIZE\n"
             "-v,--verbose verbose output\n"
             "-V,--version display version and exit\n"
             "-h,--help display this help and exit\n");
      help = true;
      break;

    case 'V':
      printf("sudoku %d.%d.%d\n"
             "Solve/generate sudoku grids (possible sizes: 1, 4, 9, 16, 25, "
             "36, 49, 64\n   ",
             VERSION, SUBVERSION, REVISION);
      version = true;
      break;

    case 'v':
      verbose = true;
      break;

    case 'a':
      all = true;
      break;

    case 'u':
      unique = true;
      break;

    case 'o':
      if (optarg == NULL) {
        errx(EXIT_FAILURE, "output file forgive") u
      } else {
        char *char_output_file = optarg;
        fprintf(stdout, "%s", char_output_file);
        output_file = fopen(char_output_file, "w");

        if (output_file == NULL) {
          errx(EXIT_FAILURE, "the file doesn't exist.");
        }
        // fclose(output_file);
      }
      break;

    case 'g':
      if (optarg == NULL) {
        size = 9;
      } else {
        size = atoi(optarg);
        if (!check_size(size)) {
          errx(EXIT_FAILURE, "Invalid size");
        }
      }
      switch (size) {
      case 1:
        break;

      case 4:
        break;

      case 9:
        break;

      case 16:
        break;

      case 25:
        break;

      case 36:
        break;

      case 49:
        break;

      case 64:
        break;

      default:
        errx(EXIT_FAILURE, "size isn't valid");
      }
      generator = true;
      break;

    default:
      errx(EXIT_FAILURE, "error: invalid option !");
    }
  }
  if (generator) {
    if (unique) {
      errx(EXIT_FAILURE, "generator and unique impossible");
    }
    if (all) {
      errx(EXIT_FAILURE, "generator and all impossible");
    }
    if (optind != argc) {
      errx(EXIT_FAILURE, "Don't put file in generator mode");
    }
  } else {
    if (optind >= argc && help == false && version == false) {
      errx(EXIT_FAILURE,
           "No file but we are in solver mode, optind %ld argc %ld", optind,
           argc);
    }
  }

  if (unique && optind == argc) {
    errx(EXIT_FAILURE, "None grid to solve");
  }

  if (!generator) {
    struct grid_t *grid;
    int optindex = optind;

    int z = 0;

    while (optindex <= argc - 1) {
      grid = file_parser(argv[optindex]);
      if (grid != NULL) {
        grid_print(grid, output_file);
        grid_free(grid);
        fclose(output_file);

      } else {
        exit_failure = true;
      }
      optindex++;
    }
  }
  if (exit_failure) {
    fprintf(stderr, "|||exit_failure|||");
    return 1;
  } else {
    return EXIT_SUCCESS;
  }
}
