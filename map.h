#ifndef MAP_HEADER
#define MAP_HEADER
#include "defs.h"

typedef struct _map map;
map *create_map();
map* assign_to_map(map* new_map, int* linedata);
int get_nlines(map* new_map);
int get_ncols(map* new_map);
int get_nislas(map* new_map);
int get_typevar(map* new_map);
void print_map(map* new_map);
void free_map(map* new_map);

#endif