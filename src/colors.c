#include "colors.h"
#include <stdlib.h>
#include <err.h>
#include <stdio.h>
/*
colors_t colors_full(const size_t size) { // a revoir
	if(size>=0 && size < MAX_COLORS)
		return ((colors_full(size-1))1 << size);
	else
		return (colors_t)1;
}*/
colors_t colors_full(const size_t size) {
  if (size <= 63) {
    return (((colors_t)1 << size) - 1);
  }
  return (((colors_t)1 << MAX_COLORS) - 1);
}

colors_t colors_empty()//a revoir
{
	return (colors_t)0;
}

colors_t colors_set (const size_t color_id)//a revoir
{
	if(color_id > MAX_COLORS)
	{
		return colors_empty();
	}
	return ((colors_t)1 << color_id);
}

colors_t colors_add (const colors_t colors, const size_t color_id)// a revoir
{
	if(color_id > MAX_COLORS)
	{
		return colors;
	}
	return (colors_t)(colors | ((colors_t)1 << (color_id)));
}

colors_t colors_discard (const colors_t colors, const size_t color_id)
{
	if(color_id > MAX_COLORS)
	{
		return colors;
	}
	return (colors_t)(colors ^ ((colors_t)1 << (color_id)))&colors;
}

bool colors_is_in (const colors_t colors, const size_t color_id)
{
	if(color_id > MAX_COLORS)
	{
		return false;
	}
	return (colors & ((colors_t)1 << (color_id))) != 0;
}

colors_t colors_negate (const colors_t colors)
{
	return ~colors;
}

colors_t colors_and (const colors_t colors1, const colors_t colors2)
{
	return colors1 & colors2;
}

colors_t colors_or (const  colors_t colors1, const colors_t colors2)
{
	return colors1 | colors2;
}

colors_t colors_xor (const  colors_t colors1, const colors_t colors2)
{
	return colors1 ^ colors2;
}

colors_t colors_subtract (const  colors_t colors1, const colors_t colors2)
{
	return colors1 ^ (colors1 & colors2);
}

bool colors_is_equal (const  colors_t colors1, const colors_t colors2)
{
	return (colors1 ^ colors2) == 0;
}

bool colors_is_subset(const  colors_t colors1, const colors_t colors2)
{
	if (colors1 == colors_empty())
	{
		return true;
	}
	if(colors1 == 0 || colors2 == 0)
	{
		return false;
	}
	return (colors1 & colors2) == colors1;
}

bool colors_is_singleton (const colors_t colors)// a revoir
{
  if (colors == 0) {
    return false;
  }

  return (colors & (colors - 1)) == 0;
}

size_t colors_count (const colors_t colors)//a revoir https://stackoverflow.com/questions/30404905/what-is-this-getproccount
{
	uint64_t color = colors;
	//basically groups off the bits of t into 2 bits and replaces each group with the count of the number of set bits in that group.
	color = color - ((color >> 1) & 0x5555555555555555ull);
	color = (color & 0x3333333333333333ull) + (color >> 2 & 0x3333333333333333ull);
	return ((color + (color >> 4)) & 0xf0f0f0f0f0f0f0full) * 0x101010101010101ull >> 56;
}

colors_t colors_rightmost (const colors_t colors)// a revoir
{
	return (colors & (-colors));
}

colors_t colors_leftmost(const colors_t colors)
{
	if (colors == 0)
	{
		return colors_empty();
	}
	int pos = 0;
	while ((colors >> pos) > 0 && pos < MAX_COLORS)
	{
		pos++;
	}

	return colors_set(pos-1);
}

colors_t colors_random(const colors_t colors)
{
  if (colors == 0) {
    return colors_empty();
  } else {
    size_t r = rand() % (colors_count(colors));
    r++;
    //fprintf(stdout,"_____r == %d\n",r);
    for(int i =0; i<MAX_COLORS; i++)
    {
    	//fprintf(stdout,"_____i == %s\n",colors);
      if (colors_is_in(colors, i))
      {
        r--;
        if (r == 0) 
        {
          
          return (colors_set(i));
        } 
      }
    }
  }
}
