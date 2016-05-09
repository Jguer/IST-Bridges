#include "isla.h"

struct _isla {
    unsigned int name;
    unsigned int bridges_avb;
    unsigned int bridges_still_avb;
    item used[4];
    pos *position;
    isla *adj[4];
    unsigned int dfsed; /* Visited flag for big group operations */
};

isla *create_isla(int name, int y, int x, int bridges_avb)
{
    isla *new_isla;
    int i = 0;

    /* Allocate isla */
    new_isla = (isla *)malloc(sizeof(isla));
    if(new_isla == NULL)
        memory_error("Unable to create structure isla");

    /* Set isla parameters */
    new_isla->position = create_pos(x,y);
    new_isla->name = name;
    new_isla->bridges_avb = bridges_avb;
    new_isla->bridges_still_avb = bridges_avb;
    new_isla->dfsed = 0;

    for(i=0; i<4; i++)
    {
        new_isla->adj[i] = NULL;
    }

    return new_isla;
}

void print_isla(item got_item)
{
    isla *got_isla = (isla *)got_item;

    fprintf(DEBUG_LOC,
            KBLU "Isla name:" RESET " %d "
            KBLU "Bridges Available:" RESET " %d ",
            got_isla->name, got_isla->bridges_avb);
    return;
}

int get_name_isla(isla *got_isla)
{
    return got_isla->name;
}

pos *get_pos_isla(isla *got_isla)
{
    return got_isla->position;
}

isla *get_adj_isla(isla *got_isla, int index)
{

    return got_isla->adj[index];
}

int get_dfs_status_isla(isla *got_isla)
{
    return got_isla->dfsed;
}

int get_bridges_s_available(isla *got_isla)
{
    return got_isla->bridges_still_avb;
}

void set_adj_isla(isla *got_isla, isla *adj_isla, int index)
{
    got_isla->adj[index] = adj_isla;
    return;
}


void free_isla(item got_item)
{
    isla *got_isla = (isla *)got_item;

    free_pos(got_isla->position);

    /* Bring democracy to the pointer */
    free(got_isla);

    return;
}
