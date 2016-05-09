#ifndef BRIDGE_HEADER
#define BRIDGE_HEADER

#include <time.h>
#include "file.h"

typedef struct _bridge bridge;

bridge *create_bridge(isla *isla_eins, isla *isla_zwei);
isla *get_points(bridge *got_bridge, int index);
int get_bridges_n_bridges(bridge* got_bridge);
void free_bridge(bridge *got_bridge);

void find_adj(map* got_map);
void find_adj_y(isla* active_row_node, map *got_map);
isla* find_next_isla_y(map *got_map, int static_x, int y, int y_max);
isla* find_next_isla_x(map *got_map, int x, int static_y, int x_max);
void print_adj(list* isla_list);

bool crossed_fire(isla* isla_a, isla* isla_b, map *got_map);
bridge* search_for_bridge_inLine(map* got_map, int isla_x, int static_y);
bridge* search_for_bridge_inCol(map* got_map, int isla_y, int static_x);

#endif
