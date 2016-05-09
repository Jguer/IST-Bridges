#include "bridge.h"

bool is_connectable(isla *isla_a, isla *isla_b, int adj_index, map *got_map);
void DFS_engine(list *head, isla *edgy, bool *visited, map* got_map);
