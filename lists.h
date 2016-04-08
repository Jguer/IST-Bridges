#ifndef STACK_HEADER
#define STACK_HEADER
#include "defs.h"
typedef struct _stack stack;
typedef struct _node node;
typedef struct _list list;

/* LIST */
list *create_list();
node *get_head(list *got_list);
void push_node_to_list(list *got_list, node *got_node);

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
