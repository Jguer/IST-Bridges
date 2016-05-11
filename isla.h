#ifndef ISLA_HEADER
#define ISLA_HEADER

#include "defs.h"
typedef struct _isla isla;

/* ----- ISLA ----- */
/* - Object -*/
isla *create_isla(int name, int y, int x, int bridges_avb);
void print_isla(item got_item);

/* - Get - */
int  get_isla_name(isla *got_isla);
item get_isla_used_bridge(isla* got_isla, int index);
int  get_isla_bridges_avb(isla *got_isla);
pos  *get_isla_pos(isla *got_isla);
isla *get_isla_adj(isla *got_isla, int index);
int  get_isla_dfs_status(isla *got_isla);
int  get_isla_bridge_s_avb(isla *got_isla);
int  get_adj_number(isla* new_isla);

/* - Set - */
void set_isla_used_bridge(isla *got_isla, int index, item connected_bridge);
void inc_isla_bridge_s_avb(isla *got_isla);
void set_isla_dfs_status(isla *got_isla, int value);
void dec_isla_bridge_s_avb(isla *got_isla);
void set_isla_adj(isla *got_isla, isla *adj_isla, int index);
#endif
