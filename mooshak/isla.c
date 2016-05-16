#include "isla.h"

struct _isla {
    isla *adj[4];
    item used[4][2];
    pos *position;
    unsigned int name;
    unsigned int bridges_avb;
    unsigned int bridges_still_avb;
    unsigned int dfsed; /* Visited flag for big group operations */
};

isla *create_isla(int name, int y, int x, int bridges_avb)
{
    isla *new_isla;
    int dir = 0;
    int i   = 0;

    /* Allocate isla */
    new_isla = (isla *)malloc(sizeof(isla));
    if(new_isla == NULL)
        memory_error("Unable to create structure isla");

    /* Set isla parameters */
    new_isla->position          = create_pos(x,y);
    new_isla->name              = name;
    new_isla->bridges_avb       = bridges_avb;
    new_isla->bridges_still_avb = bridges_avb;
    new_isla->dfsed             = 0;

    for(dir = 0; dir < 4; dir++)
    {
        new_isla->adj[dir] = NULL;

        for(i = 0; i < 2; i++ )
        {
            new_isla->used[dir][i] = NULL;
        }
    }

    return new_isla;
}

void print_isla(item got_item)
{
    isla *got_isla = (isla *)got_item;

    fprintf(DEBUG_LOC,
            KBLU "Isla name:" RESET " %d "
            KBLU "Bridges Avb:" RESET " %d "
            KBLU "Bridges still Avb:" RESET " %d ",
            got_isla->name, got_isla->bridges_avb, got_isla->bridges_still_avb);
    return;
}

int get_isla_name(isla *got_isla)
{
    return got_isla->name;
}

int get_isla_bridges_avb(isla *got_isla)
{
    return got_isla->bridges_avb;
}

item get_isla_used_bridge(isla *got_isla, int dir, int index)
{
    return got_isla->used[dir][index];
}

void set_isla_used_bridge(isla *got_isla, int dir, int index, item connected_bridge)
{
    got_isla->used[dir][index] = connected_bridge;
    return;
}

pos *get_isla_pos(isla *got_isla)
{
    return got_isla->position;
}

isla *get_isla_adj(isla *got_isla, int index)
{

    return got_isla->adj[index];
}

int get_isla_dfs_status(isla *got_isla)
{
    return got_isla->dfsed;
}

void set_isla_dfs_status(isla *got_isla, int value)
{
    got_isla->dfsed = value;
    return;
}

int get_isla_bridge_s_avb(isla *got_isla)
{
    return got_isla->bridges_still_avb;
}

void dec_isla_bridge_s_avb(isla *got_isla)
{
    if(get_isla_bridge_s_avb(got_isla) == 0)
    {
        printf("You are trying to decrease 0.\n");
        return;
    }
    got_isla->bridges_still_avb --;
    return;
}

void inc_isla_bridge_s_avb(isla *got_isla)
{
    got_isla->bridges_still_avb ++;

    if( get_isla_bridges_avb(got_isla) < get_isla_bridge_s_avb(got_isla) )
    {
        printf("You are trying to increase beyond bridges_avb.\n");
    }
    return;
}

void set_isla_adj(isla *got_isla, isla *adj_isla, int index)
{
    got_isla->adj[index] = adj_isla;
    return;
}

int get_adj_number(isla* new_isla)
{
    int n_adj  = 0;
    int i      = 0;
    isla* _adj = NULL;

    for(i = 0; i < 4; i++)
    {
        _adj = get_isla_adj(new_isla, i);

        if(_adj != NULL)
            n_adj++;
    }
    return n_adj;
}

void reset_dfsed_values(list *isla_list)
{
    isla *new_isla;
    node *new_node;

    new_node = get_head(isla_list);

    while(new_node != NULL)
    {
        new_isla = get_node_item(new_node);

        set_isla_dfs_status(new_isla, 0);

        new_node = get_next_node(new_node);
    }
    return;
}

int is_isla_greater_avb(item item_a, item item_b)
{
    isla *isla_a;
    isla *isla_b;
    isla_a = (isla *)item_a;
    isla_b = (isla *)item_b;

    if(isla_a->bridges_still_avb > isla_b->bridges_still_avb)
    {
        return 1;
    }

    return 0;
}

isla *get_isla_for_dfs(list *isla_list)
{
    isla *got_isla;
    node *aux_node;

    for(aux_node = get_head(isla_list); aux_node != NULL; aux_node = get_next_node(aux_node))
    {
        got_isla = (isla *)get_node_item(aux_node);
        if((get_isla_dfs_status(got_isla) < 1) && (get_isla_bridge_s_avb(got_isla) != 0))
        {
            return got_isla;
        }
    }

    return NULL;
}

