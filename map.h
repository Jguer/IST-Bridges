#ifndef MAP_HEADER
#define MAP_HEADER
#include "isla.h"

typedef struct _map map;
map *create_map(int x_max, int y_max, int n_bridges, int map_mode);
isla *get_tile(map *got_map, int x, int y);
void set_tile(map *got_map, isla* isla_to_add);
int get_n_bridges(map* got_map);
int get_x_max(map* got_map);
int get_y_max(map* got_map);
void print_map(int x_max, int y_max, map* got_map);
#endif
