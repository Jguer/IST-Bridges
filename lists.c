#include "lists.h"
struct _stack {
    int size;
    node *head;
};

struct _node {
    item this;
    node *next;
};

struct _list {
    node *head;
};

/* LIST */

list *create_list()
{
    list *new_list = NULL;

    /* Create List */
    new_list = (list *)malloc(sizeof(list));
    if(new_list == NULL)
        memory_error("Unable to reserve list memory");

    /* Set head to null */
    new_list->head = NULL;
    new_list->

    return new_list;
}

node *get_head(list *got_list)
{
    return got_list->head;
}

void push_node_to_list(list *got_list, node *got_node)
{
    got_node->next = got_list->head;
    got_list->head = got_node;

    return;
}
/* STACK */

stack *create_stack()
{
    stack *new_stack = NULL;

    new_stack = (stack *)malloc(sizeof(stack));
    if(new_stack == NULL)
        memory_error("Unable to reserve stack memory");

    new_stack->size = 0;
    new_stack->head = NULL;

    return new_stack;
}

void push_to_stack(stack *got_stack, item new_item)
{
    /* Node Creation */
    node *new_node;
    new_node = create_node(new_item, got_stack->head);

    /* Increase size of Stack */
    got_stack->size++;

    return;
}

node *pop_from_stack(stack *got_stack)
{
    node *got_node;

    /* Get stack head*/
    got_node = got_stack->head;

    /* Change stack head to next node*/
    got_stack->head = got_node->next;

    /* Reduce stack size */
    got_stack->size--;
    return got_node;
}

void free_stack(stack *got_stack, void (*free_item)(item))
{
    node *got_node = NULL;

    /* Free every node of stack*/
    for(got_node = pop_from_stack(got_stack);
        got_node != NULL;
        got_node = pop_from_stack(got_stack))
    {
        free_node(got_node, free_item);
    }

    /* Bring freedom to stack */
    free(got_stack);

    return;
}

/* NODE */

node *create_node(item new_item, node *next_node)
{
    node *new_node = NULL;

    /* Memory allocation */
    new_node = (node *)malloc(sizeof(node));

    if (new_node == NULL)
        memory_error("Unable to reserve node memory");

    /* Add item to node*/
    new_node->this = new_item;
    new_node->next = next_node;

    return new_node;
}

item get_node_item(node *got_node)
{
    return got_node->this;
}

node *get_next_node(node *got_node)
{
    return got_node->next;
}

void free_node(node *got_node, void (*free_item)(item))
{
    /* Free node item*/
    free_item(got_node->this);

    /* Free the node to save on the load */
    free(got_node);

    return;
}
