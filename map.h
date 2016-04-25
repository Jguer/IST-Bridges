#ifndef MAP_HEADER
#define MAP_HEADER
#include "isla.h"

typedef struct _map map;
map *create_map(int x_max, int y_max, int n_bridges, int map_mode);
void set_tile(map *got_map, isla* isla_to_add);
#endif
