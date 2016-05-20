#ifndef MAP_HEADER
#define MAP_HEADER
#include "isla.h"

typedef struct _map map;

map *create_map(int x_max, int y_max, int n_bridges, int map_mode);
isla *get_tile(map *got_map, int x, int y);
item get_bridge_from_vector_index(map *got_map, int index, int select);
void set_bridge_from_vector_index(map *got_map, int index, int select, item new_item);
void set_tile(map *got_map, isla* isla_to_add);
item get_bridge_from_vector(map *got_map, int isla_a, int isla_b, int select);
void set_bridge_from_vector(map *got_map, int isla_a, int isla_b, int select, item new_item);
int get_x_max(map* got_map);
int get_y_max(map* got_map);
char get_map_mode(map* got_map);
int get_n_islas(map* got_map);
int get_map_mode_result(map* got_map);
void set_map_mode_result(map *got_map, int mode_result);
void print_map(map* got_map);
void print_map_graphic(map* got_map);
void free_bridge_vector(map *got_map);
void free_map(item got_item);
#endif

