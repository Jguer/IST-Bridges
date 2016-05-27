#include "tree.h"

struct t {
    item this;
    struct t *kids[4];
};

t_node *create_t_node(item got_item)
{
    int i = 0;

    t_node *new_node = (t_node *)malloc(sizeof(t_node));
    if( new_node == NULL )
    {
        memory_error("Unable to create tree_node");
    }

    for(i = 0; i < 4; i++)
    {
        kids[i] = NULL;
    }

    return new_node;
}
