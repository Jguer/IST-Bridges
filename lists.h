#ifndef STACK_HEADER
#define STACK_HEADER
#include "defs.h"
typedef struct _stack stack;
typedef struct _node node;

/* STACK */
stack *create_stack();
void push_to_stack(stack *got_stack, item new_item);
node *pop_from_stack(stack *got_stack);
void free_stack(stack *got_stack, void (*free_item)(item));

/* NODE */
node *create_node(item new_item, node *next_node);
item get_node_item(node *got_node);
void free_node(node *got_node, void (*free_item)(item));

#endif
