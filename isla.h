#ifndef ISLA_HEADER
#define ISLA_HEADER

#include "defs.h"
typedef struct _isla isla;

isla *create_isla(int name, int y, int x, int bridges_avb);

void set_adj_isla(isla *got_isla, isla *adj_isla, int index);
isla *get_adj_isla(isla *got_isla, int index);
pos *get_pos_isla(isla *got_isla);
int get_bridges_s_available(isla *got_isla);
int get_name_isla(isla *got_isla);
<<<<<<< HEAD
int get_dfs_status_isla(isla *got_isla);
=======
int get_bridges_avb(isla *got_isla);
item get_used_bridge(isla* got_isla, int index);
>>>>>>> 3df531c0b34d9c3b6f7a0cf3c7389a1ca134f69a
void print_isla(item got_item);
void free_isla(item got_item);

#endif
