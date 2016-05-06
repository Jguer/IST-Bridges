#ifndef BRIDGE_HEADER
#define BRIDGE_HEADER

#include <time.h>

#include "file.h"

void find_adj(map* got_map);
void find_adj_y(isla* active_row_node, map *got_map);
isla* find_next_isla_y(map *got_map, int static_x, int y, int y_max);
isla* find_next_isla_x(map *got_map, int x, int static_y, int x_max);
void print_adj(list* isla_list);

#endif
