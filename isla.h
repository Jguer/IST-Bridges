#ifndef ISLA_HEADER
#define ISLA_HEADER

#include "defs.h"
typedef struct _isla isla;

isla *create_isla(int name, int y, int x, int bridges_avb);

int get_isla_name(isla *got_isla);
item get_isla_used_bridge(isla* got_isla, int index);
void print_isla(item got_item);

int get_isla_bridges_avb(isla *got_isla);
pos *get_isla_pos(isla *got_isla);
void set_isla_used_bridge(isla *got_isla, int index, item connected_bridge);
isla *get_isla_adj(isla *got_isla, int index);
int get_isla_dfs_status(isla *got_isla);
void set_isla_dfs_status(isla *got_isla, int value);
int get_isla_bridge_s_avb(isla *got_isla);
void dec_isla_bridge_s_avb(isla *got_isla);
void set_isla_adj(isla *got_isla, isla *adj_isla, int index);
#endif
