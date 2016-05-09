#include "search_engine.h"

bool is_connectable(isla *isla_eins, isla *isla_zwei)
{
    return TRUE;
}

void engine_static_fire()
{

}


isla *get_isla_for_dfs(list *isla_list)
{
    isla *got_isla;
    node *aux_node;

    for(aux_node = get_head(isla_list); aux_node != NULL; aux_node = get_next_node(aux_node))
    {
        got_isla = (isla *)get_node_item(aux_node);
        if((get_dfs_status_isla(got_isla) == 0) && ( get_bridges_s_available(got_isla) != 0))
        {
            return got_isla;
        }
    }

    return NULL;
}

void DFS_manager(list *isla_list, int mode)
{

    bool *visited = (bool *) calloc(get_list_size(isla_list), sizeof(bool));
    isla *good_isla;

    if(mode == 1)
    {
        good_isla = get_isla_for_dfs(isla_list);
        while( good_isla != NULL ) {
            DFS_engine(get_node_item(get_head(isla_list)), visited);
            memset(visited, FALSE , sizeof(bool) * get_list_size(isla_list));
            good_isla = get_isla_for_dfs(isla_list);
        }
    }
    else if(mode == 2)
    {

    }
    else if(mode == 3)
    {
        DFS_engine(get_node_item(get_head(isla_list)), visited);
    }
    else
    {
        /* Invalid Mode */
        fprintf(stderr, KYEL "Good Job, you officially failed at map making. " KRED " Invalid mode\n"KNRM);
    }

    free(visited);
    return;
}

void DFS_engine(isla *edgy, bool *visited)
{
    isla *_adj = NULL;
    unsigned int i = 0;
    visited[get_name_isla(edgy)] = TRUE;

    /* Travel all nodes, list implementation may be underkill*/
    /* i gives NSEW*/
    for(_adj = get_adj_isla(edgy, i) ; i < 4; i++)
    {
        /* Check if exists and check if visited*/
        if(_adj != NULL )
        {
            /* If islas are good for connect*/
            if(is_connectable(edgy, _adj) == TRUE)
            {

            }
            if( visited[get_name_isla(_adj)] != TRUE)
            {
                DFS_engine(_adj, visited);
            }
        }
    }

    return;
}

void engine_dry_run()
{

}


