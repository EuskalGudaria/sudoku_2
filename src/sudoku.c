#include "sudoku.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <err.h>
#include <getopt.h>

static bool verbose = false;
static FILE *output_file = NULL;

int main(int argc, char **argv) {
  // Bool we use to detect in wich mode we are
  bool all = false;
  bool unique = false;
  bool generate = false;

  static struct option long_opts[] = {{"help", no_argument, 0, 'h'},
                                      {"version", no_argument, 0, 'V'},
                                      {"verbose", no_argument, 0, 'v'},
                                      {"output", required_argument, 0, 'o'},
                                      {"all", no_argument, 0, 'a'},
                                      {"unique", no_argument, 0, 'u'},
                                      {"generate", required_argument, 0, 'g'}};

  static char *optString =
      "ag:ho:uvV"; // Here we specify that option "g" and "o" requrired an arguments, and the programm print an error if no arguments is given
  int optc;

  while ((optc = getopt_long(argc, argv, optString, long_opts, NULL)) != -1) {
    switch (optc) {
    case 'a':
      all = true;
      break;
    case 'g':
      generate = true;
      break;

    case 'h':
      fputs("Usage: sudoku [-a|-o FILE|-v|-V|-h] FILE...\n"
            "       sudoku [-g SIZE] [-u|-o FILE|-v|-V|-h]\n"
            "Solve or generate Sudoku grids of various sizes "
            "(1,4,9,16,25,36,49,64)\n"
            "\n"
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
      break;
    }
  }

  // get all FILE in arguments
  char *file_arg_list[MAX_FILES]; //! Store files

  int nb_file = 0;
  while (argv[optind + nb_file]) {
    file_arg_list[nb_file] = argv[optind + nb_file];
    nb_file++;
  }

  // Code with the aim to detect in wich mode we are
  if (!generate) // Code detect that we are in solver mode we will check below if we have inconsistent option (unique)
  {
    printf("---Solver mode---\n");
    if (nb_file) {
      // In solver mode we have to check if files are given on arguments and how many
      bool err_exit = false;
      for (int i = 0; i < nb_file; i++) //
      {
        // Browsing all file to check if they are readable
        if (access(file_arg_list[i], R_OK)) {
          printf("error: %s is not readable\n",
                 file_arg_list[i]); // If file is not readable we print an error with the specify unreadable
          err_exit = true;
        }
      }
      if (err_exit) // Exit the programm if one file is unreadable
      {
        exit(EXIT_FAILURE);
      }
    } else // Case where no file are given in arguments
    {
      errx(EXIT_FAILURE, "error: no input grid given!");
    }

    if (unique) // Detect about an inconsistent option and print the issue to the user
    {
      warnx(
          "warning: option 'unique' conflicts with solver mode, disabling it!");
      unique = false;
    }
  } else // If we are not in solver mode we consider to be in generate mode
  {
    printf("---Generator mode---%s mode\n", unique ? "unique" : "normal");

    if (all) // Detect about an inconsistent option and print the issue to the user
    {
      warnx(
          "warning: option 'all' conflicts with generator mode, disabling it!");
      all = false;
    }
  }

  if (output_file) // If the option -o is used the output is redirect into the output file and we need to close that output file
  {
    fclose(output_file);
  }
  return 0;
}
