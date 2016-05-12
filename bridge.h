#ifndef BRIDGE_HEADER
#define BRIDGE_HEADER
#include "map.h"
#include "lists.h"

typedef struct _bridge bridge;
/* ----- BRIDGE ----- */
/* - Object -*/
bridge *create_bridge(isla *isla_eins, isla *isla_zwei);
void   print_bridge(item got_item);
void   free_bridge(item);

/* - Get - */
isla *get_points(bridge *got_bridge, int index);
int  get_numberof_bridges(list *isla_list);
list *get_bridge_probi_list(bridge *got_bridge);
int get_isla_n_bridges(isla *new_isla, int dir);

/* - Set - */

/* ----- ADJ ----- */
/* - Object - */
void print_adj(list* isla_list);
void free_isla(item got_item);

/* - Logic - */
int     get_numberof_bridges_avb(list *isla_list);
int     get_numberof_bridges(list *isla_list);
void    find_adj(map* got_map);
void    find_adj_y(isla* active_row_node, map *got_map);
bool    crossed_fire(isla* isla_a, isla* isla_b, map *got_map);
isla*   find_next_isla_y(map *got_map, int static_x, int y, int y_max);
isla*   find_next_isla_x(map *got_map, int x, int static_y, int x_max);
bridge* search_for_bridge_inLine(map* got_map, int isla_x, int static_y);
bridge* search_for_bridge_inCol(map* got_map, int isla_y, int static_x);
bridge  *connect_islas(isla *isla_a, isla *isla_b, int index);
bool initial_fuck_up(list *isla_list);

#endif
