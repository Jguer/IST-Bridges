#ifndef ISLA_HEADER
#define ISLA_HEADER

#include "defs.h"
typedef struct _isla isla;

isla *create_isla(int name, int x, int y, int bridges_avb);
void print_isla(item got_item);
void free_isla(item got_item);

#endif
