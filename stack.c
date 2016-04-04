#include "stack.h"
struct _stack {
    item this;
    int size;
    list *head;
};

struct _list {
    item this;
    list *next;
};

void push_to_stack(stack *got_stack, item new_item)
{
    list *new_list;

    /* Memory allocation */
    new_list = (list *)malloc(sizeof(list));

    if (new_list == NULL)
        memory_error("Unable to reserve memory");

    /* Prepend Node*/
    new_list->this = new_item;
    new_list->next = got_stack->head;
    got_stack->head = new_list;

    return;
}
