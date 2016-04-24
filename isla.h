#ifndef ISLA_HEADER
#define ISLA_HEADER

#include "defs.h"
typedef struct _isla isla;

isla *create_isla(int name, int bridges_avb);

void set_adj_isla(isla *got_isla, isla *adj_isla, int index);
isla *get_adj_isla(isla *got_isla, int index);
void print_isla(item got_item);
void free_isla(item got_item);

#endif
