#include "search_engine.h"

bool is_connectable(isla *isla_a, isla *isla_b, int adj_index, map *got_map)
{
    bridge *new_bridge = NULL;

    if(get_bridges_avb(isla_a)>0 && get_bridges_avb(isla_b)>0)
    {
        new_bridge = (bridge*)get_used_bridge(isla_a, adj_index);

        /* Just a little test for now -----------------*/
        if(get_bridges_n_bridges(new_bridge) > 0)
        {
            if(get_points(new_bridge, 0) != isla_a || get_points(new_bridge, 0) != isla_b)
            {
                printf("SOMETHING IS VERY WRONG. CHECK is_connectable\n");
            }  
        }
        /*--------------------------------------------*/

        if(get_bridges_n_bridges(new_bridge) >= 2)
            return FALSE;

        if(crossed_fire(isla_a, isla_b, got_map))
            return FALSE;
        return TRUE;

    }

    return FALSE;
}

void engine_static_fire()
{

}

void DFS_engine(list *head, isla *edgy, bool *visited, map* got_map)
{
    list *auximoron;
    isla *_adj = NULL;
    unsigned int i = 0;
    visited[get_name_isla(edgy)] = TRUE;

    printf("Digging Isla %d \n", get_name_isla(edgy));

    /* Travel all nodes, list implementation may be underkill*/
    /* for(auximoron = head; auximoron != NULL; auximoron ); */
    for(_adj = get_adj_isla(edgy, i) ; i < 4; i++)
    {
        printf("Checking for Isla %d, I: %d \n", get_name_isla(edgy),i);
        /* Check if exists and check if visited*/
        if(_adj != NULL )
        {
            printf("Howdy Neighbour \n Flanders %d - %p \n", get_name_isla(_adj), _adj);
            /* If islas are good for connect*/
            if(is_connectable(edgy, _adj, i, got_map) == TRUE)
            {
            }
            if(visited[get_name_isla(_adj)] != TRUE)
            {
                DFS_engine(head, _adj, visited, got_map);
            }
        }

    }

    return;
}


void engine_dry_run()
{

}


