#include "map.h"

struct _map {

	int nlines;
	int ncols;
	int nislas;
	int typevar;
};

map *create_map()
{
    map *new_map;
    new_map = (map *)malloc(sizeof(map));
    if(new_map == NULL)
    	memory_error("Unable to create structure map");

    new_map->nlines = 0;
    new_map->ncols = 0;
    new_map->nislas = 0;
    new_map->typevar = 0;

    return new_map;
}

map* assign_to_map(map* new_map, int* linedata)
{
	assign_values(linedata, &new_map->nlines, &new_map->ncols, &new_map->nislas, &new_map->typevar, NULL);
	return new_map;
}

int get_nlines(map* new_map)
{
  return new_map->nlines;
}
int get_ncols(map* new_map)
{
  return new_map->ncols;
}
int get_nislas(map* new_map)
{
  return new_map->nislas;
}
int get_typevar(map* new_map)
{
  return new_map->typevar;
}

void print_map(map* new_map)
{
	fprintf(DEBUG_LOC,
            KGRN "nlines:" RESET " %d "
            KGRN "ncols:" RESET " %d "
            KGRN "nislas:" RESET " %d " 
            KGRN "variante:" RESET " %d\n",
            new_map->nlines, new_map->ncols, new_map->nislas, new_map->typevar);
    return;
}

void free_map(map* new_map)
{
  free(new_map);
  return;
}