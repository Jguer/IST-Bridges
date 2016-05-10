#include "bridge.h"

bool is_connectable(isla *isla_a, isla *isla_b, int adj_index, map *got_map);
stack *DFS_manager(list *isla_list, int mode, map* got_map);
bool check_for_allconnected(list *isla_list);
