#include "bridge.h"

stack *DFS_manager(list *isla_list, int mode, map* got_map);
bool check_for_allconnected(list *isla_list);
stack *gen_essential_bridges(list *isla_list);
