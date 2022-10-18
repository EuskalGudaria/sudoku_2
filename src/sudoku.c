#include "sudoku.h"
#include "colors.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <err.h>
#include <getopt.h>
#include <string.h>

static bool verbose = false;
static FILE *output_file = NULL;
const char *characters =
    "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ@abcdefghijklmnopqrstuvwxyz&*";

grid_t *grid_alloc(size_t size) // allocate the memory needed
{
  grid_t *grid = malloc(sizeof(grid_t));

  grid->size = size;
  grid->cells = calloc(size, sizeof(char *));

  for (int i = 0; i < (int)size; i++) {
    grid->cells[i] = calloc(size, sizeof(char *));

    for (int j = 0; j < (int)size; j++) {
      grid->cells[i][j] = '_';
    }
  }
  return grid;
}

void grid_free(grid_t *grid) {
  for (size_t i = 0; i < (size_t)grid->size; i++) {
    free(grid->cells[i]);
  }

  free(grid->cells);
  free(grid);
}

void grid_print(const grid_t *grid, FILE *fd) {
  for (size_t i = 0; i < (size_t)grid->size; i++) {
    for (size_t j = 0; j < (size_t)grid->size; j++) {
      fprintf(fd, "%c", grid->cells[i][j]);
    }
    fprintf(fd, "\n");
  }
}

bool check_char(const grid_t *grid, const char c) {
  if (c == '_') {
    return true;
  }
  for (size_t i = 0; i < (size_t)grid->size; i++) {
    if (c == characters[i]) {
      return true;
    }
  }
  return false;
}

grid_t *file_parser(char *filename) {
  FILE *file = fopen(filename, "r");

  if (!file) {
    warnx("error: file %s doesn't exist or isn't accessible for reading",
          filename);
    return NULL;
  }

  char *line = NULL;
  size_t length = 0;
  int check;
  do {
    check = getline(&line, &length, file);
  } while (check != -1 && (line[0] == '#' || line[0] == '\n'));

  if (check == -1) // EOF
  {
    free(line);
    fclose(file);
    warnx("error: no grid found");
    return NULL;
  }

  size_t size = 0;

  int i = 0;
  while (line[i] != '\n' && line[i] != '\0') {
    if (line[i] == '#') {
      break;
    } else if (strchr(characters, line[i]) || line[i] == '_') {
      size++;
    }
    i++;
  }

  free(line);
  rewind(file);

  if (!(size == 1 || size == 4 || size == 9 || size == 16 || size == 25 ||
        size == 36 || size == 49 || size == 64)) {
    fclose(file);
    warnx("error: grids of size %ld are not supported", size);
    return NULL;
  }

  grid_t *grid = grid_alloc(size);

  char c;
  int row_nbr = 0, col_nbr = 0;
  bool line_formated;

  do {
    c = fgetc(file);

    if (c == EOF) {
      break;
    }

    line_formated = false;

    if (c == '#') {
      do {
        c = fgetc(file);
      } while (c != '\n' && c != EOF);
      if (!col_nbr) {
        col_nbr = size;
        line_formated = true;
      }
    }

    if (c == '\n' && col_nbr != 0) {
      if (col_nbr != (size_t)size) {
        fclose(file);
        grid_free(grid);
        warnx("error: line %d is malformed! (wrong number of columns",
              row_nbr + 1);
        return NULL;
      }
      col_nbr = 0;
      row_nbr += !line_formated;
    }

    if (check_char(grid, c)) {
      if (col_nbr >= (size_t)size) {
        fclose(file);
        grid_free(grid);
        warnx("error: line %d is malformed! (wrong number of columns",
              row_nbr + 1);
        return NULL;
      }

      grid->cells[row_nbr][col_nbr] = c;
      col_nbr++;
    } else if (strchr(characters, c)) {
      fclose(file);
      grid_free(grid);
      warnx("error: wrong character %c at line %d", c, row_nbr + 1);
      return NULL;
    }

  } while (row_nbr < (int)size);

  if (feof(file) && c != '\n') {
    row_nbr++;
  }

  if (row_nbr < (size_t)size) {
    fclose(file);
    grid_free(grid);
    warnx("error: grid has %ld missing line(s)!", size - row_nbr);
    return NULL;
  }

  if (feof(file) && col_nbr != (size_t)size) {
    fclose(file);
    grid_free(grid);
    warnx("error: line %d is malformed! (wrong number of columns", row_nbr + 1);
    return NULL;
  }

  do {
    c = fgetc(file);

    if (c == '#') {
      do {
        c = fgetc(file);
      } while (c != '\n' && c != EOF);
    }

    if (strchr(characters, c)) {
      fclose(file);
      grid_free(grid);
      warnx("error: grid in file %s has too much rows", filename);
      return NULL;
    }
  } while (c != EOF);

  fclose(file);

  grid_print(grid, output_file ? output_file : stdout);
  return grid;
}

int main(int argc, char **argv) {

  bool all = false;
  bool unique = false;
  bool generate = false;
  size_t g_grid_size = 0;

  struct option long_opts[] = {{"help", no_argument, NULL, 'h'},
                               {"version", no_argument, NULL, 'V'},
                               {"verbose", no_argument, NULL, 'v'},
                               {"output", required_argument, NULL, 'o'},
                               {"all", no_argument, NULL, 'a'},
                               {"unique", no_argument, NULL, 'u'},
                               {"generate", optional_argument, NULL, 'g'},
                               {NULL, 0, NULL, 0}};

  static char *opt_string = "ag::ho:uvV";
  int optc;

  while ((optc = getopt_long(argc, argv, opt_string, long_opts, NULL)) != -1) {
    switch (optc) {
    case 'a':
      all = true;
      break;

    case 'g':
      generate = true;
      g_grid_size = optarg ? atoi(optarg) : 9;
      break;

    case 'h':
      fputs("Usage: sudoku [-a|-o FILE|-v|-V|-h] FILE...\n"
            "       sudoku [-g SIZE] [-u|-o FILE|-v|-V|-h]\n"
            "Solve or generate Sudoku grids of various sizes "
            "(1,4,9,16,25,36,49,64)\n"
            " -a,-all                search for all possible solutions \n"
            " -g[N],--generate[=N]   generate a grid of size NxN (default:9) \n"
            " -u,--unique            generate a grid with unique solution \n"
            " -o FILE,--output FILE  write solution to FILE\n"
            " -v,--verbose           verbose output \n"
            " -V,--version           display version and exit \n"
            " -h,--help              display this help and exit \n",
            stdout);
      exit(EXIT_SUCCESS);
      break;

    case 'o':
      output_file = fopen(optarg, "w+");
      if (!output_file) {
        errx(EXIT_FAILURE, "error: could not create/read outputs %s", optarg);
      }

      break;

    case 'u':
      unique = true;
      break;

    case 'v':
      verbose = true;
      break;

    case 'V':
      printf("sudoku %d.%d.%d\n", VERSION, SUBVERSION, REVISION);
      printf("Solve or generate Sudoku grids of various sizes "
             "(1,4,9,16,25,36,49,64)\n");
      exit(EXIT_SUCCESS);
      break;

    default:
      errx(EXIT_FAILURE, "error: invalid option'%s'!", argv[optind - 1]);
    }
  }

  int nb_files = argc - optind;
  grid_t *grids[nb_files];

  if (!generate) {
    printf("---Solver mode---\n");
    if (!nb_files) {
      if (output_file) {
        fclose(output_file);
      }
      errx(EXIT_FAILURE, "error: no input grid given");
    }
    bool correct = true;

    for (int i = 0; i < nb_files; i++) {
      grids[i] = file_parser(argv[optind + i]);
      correct = correct && grids[i];
    }

    for (int i = 0; i < nb_files; i++) {
      if (grids[i]) {
        grid_free(grids[i]);
      }
    }
    
    if (!correct) {
      if (output_file) {
        fclose(output_file);
      }
      return EXIT_FAILURE;
    }

    if (unique) {
      warnx(
          "warning: option 'unique' conflicts with solver mode, disabling it!");
      unique = false;
    }
  } else {
    if (!(g_grid_size == 1 || g_grid_size == 4 || g_grid_size == 9 ||
          g_grid_size == 16 || g_grid_size == 25 || g_grid_size == 36 ||
          g_grid_size == 49 || g_grid_size == 64)) {
      if (output_file) {
        fclose(output_file);
      }
      errx(EXIT_FAILURE, "error: grids of size %ld are not supported",
           g_grid_size);
    }

    printf("---Generator mode---%s mode\n", unique ? "unique" : "normal");

    if (all) {
      warnx(
          "warning: option 'all' conflicts with generator mode, disabling it!");
      all = false;
    }
  }

  if (output_file) {
    fclose(output_file);
  }

  return EXIT_SUCCESS;
}
