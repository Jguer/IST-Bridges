#include "lists.h"
struct _stack {
    size_t size;
    node *head;
};

struct _node {
    item this;
    node *next;
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

    /* Set size to 0 :( */
    new_list->size = 0;

    return new_list;
}

node *get_head(list *got_list)
{
    return got_list->head;
}

void set_head(list *got_list, node *got_head)
{
    got_list->head = got_head;
    return;
}

void push_item_to_list(list *got_list, item new_item)
{
    /* Node Creation */
    node *new_node = NULL;
    new_node = create_node(new_item, NULL);

    /* Prepend node to list*/
    push_node_to_list(got_list, new_node);
    return;
}

void push_node_to_list(list *got_list, node *got_node)
{
    got_node->next = got_list->head;
    got_list->head = got_node;

    /* Increase List size :D */
    got_list->size ++;

    return;
}

size_t get_list_size(list *got_list)
{
    return got_list->size;
}


void print_list(list *got_list, void (*print_item)(item))
{
    node *aux_node;
    printf("Print list:\n");
    fprintf(DEBUG_LOC, "Size of list: %lu\n", get_list_size(got_list));

    for(aux_node = get_head(got_list);
            aux_node != NULL;
            aux_node = get_next_node(aux_node))
    {
        printf("[ ");
        print_item(get_node_item(aux_node));
        printf("] -> \n");
    }

    printf("-> [ " KBLU "NULL" RESET " ]");

}

void free_list(list *got_list, void (*free_item)(item))
{
    node *got_node = get_head(got_list);

    /* Free every node of list*/
    free_connected_nodes(got_node, free_item);

    /* Bring freedom to stack */
    free(got_list);

    return;
}

list *sort_list (list *to_sort, int (*cmp_list)(item, item))
{
    int num_nodes = get_list_size(to_sort);
    int counter;
    node *current;
    node *next;
    node *previous;

    for (counter = 0; counter < num_nodes; counter++) {
        current = get_head(to_sort);
        next = get_next_node(current);
        previous = NULL;

        while(next != NULL) {
            int compare = cmp_list(get_node_item(current), get_node_item(next));
            if (compare == 0) {
                if (current == get_head(to_sort)){
                    to_sort->head = next;
                } else {
                    previous->next = next;
                }
                current->next = next->next;
                next->next = current;

                previous = next;
                next = current->next;
            }
            else {
                previous = current;
                current = current->next;
                next = current->next;
            }
        }
    }
    return to_sort;
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
    node *new_node = NULL;
    new_node = create_node(new_item, got_stack->head);

    /* Increase size of Stack */
    got_stack->size++;
    got_stack->head = new_node;

    return;
}

node *pop_from_stack(stack *got_stack)
{
    node *got_node;

    /* Get stack head*/
    got_node = got_stack->head;
    if(got_node != NULL)
    {
        /* Change stack head to next node*/
        got_stack->head = got_node->next;
        got_stack->size--;
    }

    /* Reduce stack size */
    return got_node;
}

node *get_stack_head(stack *got_stack)
{
    return got_stack->head;
}

size_t get_stack_size(stack *got_stack)
{
    return got_stack->size;
}


int is_stack_empty(stack *got_stack)
{
    if(got_stack->head == NULL)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void print_stack(stack *got_stack, void (*print_item)(item))
{
    node *aux_node;

    /* Print stack size to selected DEBUG_LOC*/
    fprintf(DEBUG_LOC, "Size of stack: %lu\n", get_stack_size(got_stack));

    /* For each node print content*/
    for(aux_node = got_stack->head;
            aux_node != NULL;
            aux_node = get_next_node(aux_node))
    {
        print_item(get_node_item(aux_node));
    }
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

    /* Bring America to stack */
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
    /* Free node item */
    free_item(got_node->this);

    /* Free the node to save on the load */
    free(got_node);

    return;
}

void free_connected_nodes(node *got_node, void (*free_item)(item))
{
    node *aux_node = NULL;

    /* Free every node connected*/
    while(got_node != NULL)
    {
        aux_node = get_next_node(got_node);
        free_node(got_node, free_item);
        got_node = aux_node;
    }
    return;
}

void merge_lists(list *list_a, list *list_b)
{
    node *aux_node;
    node *final_head;
    size_t final_size;

    final_size = list_a->size + list_b->size;

    if(list_a->head == NULL)
    {
        list_a->head = list_b->head;
        list_b->head = NULL;
        return;
    }
    else if(list_b->head == NULL)
    {
        return;
    }

    if(list_a->size < list_b->size)
    {
        aux_node = list_a->head;
        final_head = list_b->head;
        list_a->size = final_size;
    }
    else
    {
        aux_node = list_b->head;
        final_head = list_a->head;
        list_b->size = final_size;
        list_a->head = list_b->head;
    }

    while(aux_node->next != NULL)
    {
        aux_node = get_next_node(aux_node);
    }

    list_b->head = NULL;
    aux_node->next = final_head;
    return;
}

void already_free(item got_item)
{
    if( got_item != NULL )
        return;
    return;
}


