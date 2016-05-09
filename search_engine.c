#include "search_engine.h"

bool is_connectable(isla *isla_a, isla *isla_b, int adj_index, map *got_map)
{
    bridge *new_bridge = NULL;

    if(get_isla_bridge_s_avb(isla_a) > 0 && get_isla_bridge_s_avb(isla_b) > 0)
    {
        new_bridge = (bridge*)get_used_bridge(isla_a, adj_index);
        if(new_bridge != NULL)
        {

            /* Just a little test for now -----------------*/
            if(get_bridges_n_bridges(new_bridge) > 0)
            {
                if(get_points(new_bridge, 0) != isla_a || get_points(new_bridge, 0) != isla_b)
                {
                    printf("\nSOMETHING IS VERY WRONG. CHECK is_connectable\n");
                }
            }
            /*--------------------------------------------*/

            if(get_bridges_n_bridges(new_bridge) >= 2)
                return FALSE;
        }

        if(crossed_fire(isla_a, isla_b, got_map))
            return FALSE;
        return TRUE;

    }

    return FALSE;
}


isla *get_isla_for_dfs(list *isla_list)
{
    isla *got_isla;
    node *aux_node;

    for(aux_node = get_head(isla_list); aux_node != NULL; aux_node = get_next_node(aux_node))
    {
        got_isla = (isla *)get_node_item(aux_node);
        if((get_dfs_status_isla(got_isla) == 0) && (get_isla_bridge_s_avb(got_isla) != 0))
        {
            return got_isla;
        }
    }

    return NULL;
}

stack *DFS_engine(isla *edgy, bool *visited, map* got_map, stack *bridge_stack)
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
            if(is_connectable(edgy, _adj, i, got_map) == TRUE)
            {
                /* Push to stack new bridge */
                push_to_stack( bridge_stack, (item) create_bridge(edgy , _adj));

                /* Augment new brigde structure count*/
                increment_bridges_n_bridges(get_node_item(get_stack_head(bridge_stack)));
            }
            if(visited[get_name_isla(_adj)] != TRUE) /* If node hasn't been visited go in*/
            {
                DFS_engine(_adj, visited, got_map, bridge_stack); /* New recursion level */
            }
        }
    }

    return bridge_stack;
}

stack *DFS_manager(list *isla_list, int mode, map* got_map)
{
    isla *good_isla;
    bool *visited = (bool *) calloc(get_list_size(isla_list) + 1, sizeof(bool));
    stack *new_stack = create_stack();

    if(mode == 1) /* */
    {
        good_isla = get_isla_for_dfs(isla_list);
        while( good_isla != NULL ) {
            DFS_engine(get_node_item(get_head(isla_list)), visited, got_map, new_stack);
            set_isla_dfs_status(good_isla, get_dfs_status_isla(good_isla) + 1);

            memset(visited, FALSE , sizeof(bool) * (get_list_size(isla_list)));
            good_isla = get_isla_for_dfs(isla_list);
        }
    }
    else if(mode == 2)
    {

    }
    else if(mode == 3)
    {
        DFS_engine(get_node_item(get_head(isla_list)), visited, got_map, new_stack);
    }
    else
    {
        /* Invalid Mode */
        fprintf(stderr, KYEL "Good Job, you officially failed at map making. " KRED " Invalid mode\n"KNRM);
    }

    print_stack(new_stack, print_bridge);
    free(visited);
    return new_stack;
}

bool check_for_allzero(list *isla_list)
{
    isla *new_isla;
    node *new_node;

    new_node = get_head(isla_list);
    new_isla = get_node_item(new_node);

    while(new_node != NULL)
    {
        if(get_isla_bridge_s_avb(new_isla) != 0)
            return FALSE;

        new_node = get_next_node(new_node);
        new_isla = get_node_item(new_node);
    }
    return TRUE;
}
