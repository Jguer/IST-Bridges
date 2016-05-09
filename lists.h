#ifndef STACK_HEADER
#define STACK_HEADER
#include "defs.h"
typedef struct _stack stack;
typedef struct _node node;
typedef struct _stack list;

/* LIST */
list *create_list();
node *get_head(list *got_list);
void push_item_to_list(list *got_list, item new_item);
void push_node_to_list(list *got_list, node *got_node);
size_t get_list_size(list *got_list);
void print_list(list *got_list, void (*print_item)(item));
void free_list(list *got_list, void (*free_item)(item));

/* STACK */
stack *create_stack();
void push_to_stack(stack *got_stack, item new_item);
node *get_stack_head(stack *got_stack);
node *pop_from_stack(stack *got_stack);
size_t get_stack_size(stack *got_stack);
int is_stack_empty(stack *got_stack);
void free_stack(stack *got_stack, void (*free_item)(item));
void print_stack(stack *got_stack, void (*print_item)(item));

/* NODE */
node *create_node(item new_item, node *next_node);
node *get_next_node(node *got_node);
item get_node_item(node *got_node);
void free_node(node *got_node, void (*free_item)(item));


void already_free(item got_item);
#endif
