#include "colors.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <err.h>
#include <getopt.h>
#include <string.h>
#include <inttypes.h>

colors_t  colors_full (const size_t size)
{   
    if (size > 63)
    {
        return(MAX_COLORS);
    }

    return (((colors_t)1<<size) - 1);
}

