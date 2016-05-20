#include "search_engine.h"

void define_visibility(list *probi_list)
{
    bridge *new_bridge = NULL;
    node *new_node = NULL;

    new_node = get_head(probi_list);
    while(new_node != NULL)
    {
        new_bridge = get_node_item(new_node);
        set_bridge_visibility(new_bridge, 0);
        new_node = get_next_node(new_node);
    }
    return;
}

isla *is_map_connectable(list *isla_list, stack *got_stack, map *got_map)
{
    node *new_node = NULL;
    isla *new_isla = NULL, *_adj = NULL;
    int dir = 0;

    new_node = get_head(isla_list);

    while(new_node != NULL)
    {
        new_isla = get_node_item(new_node);

        for(dir=0; dir<4; dir++)
        {
            _adj = get_isla_adj(new_isla, dir);
            if(_adj != NULL)
            {
                if(is_connectable(new_isla, _adj, dir, got_stack, got_map) == TRUE && get_isla_dfs_status(new_isla) == 0)
                {
                    return new_isla;
                }
            }
        }

        new_node = get_next_node(new_node);
    }
    return NULL;
}


int define_mode_result(int mode, bool is_solved, list *isla_list)
{
    int mode_result = 0;

    if(mode == 1)
    {
        if(is_solved)
            mode_result = GOT_SOL;
        else
            mode_result = NO_SOL;
    }

    else if(mode == 2)
    {
        if(is_solved)
        {
            if(check_for_allconnected(isla_list))
                mode_result = ALL_CONCT;
            else
                mode_result = NOT_ALL_CONCT;

        }
        else
            mode_result = NO_SOL;
    }

    else
    {
        if(is_solved)
            mode_result = ALL_CONCT;
        else
            mode_result = NO_SOL;
    }

    return mode_result;
}


void DFS_engine(isla *edgy, bool *visited, map* got_map, stack *bridge_stack, bool erase_invisibility)
{
    isla *_adj = NULL;
    unsigned int dir = 0;

    visited[get_isla_name(edgy) - 1] = TRUE;

    /* Travel all nodes, list implementation may be underkill*/
    /* i gives NSEW*/
    for(dir = 0; dir < 4; dir++)
    {
        _adj = get_isla_adj(edgy, dir);
        /* Check if exists, check if visited and check if islas are good for connect*/
        if(_adj != NULL && is_connectable(edgy, _adj, dir, bridge_stack, got_map) == TRUE)
        {
            /* Create new bridge and push it to stack */
            push_to_stack(bridge_stack, connect_islas(edgy, _adj, dir, got_map));
            if(erase_invisibility == TRUE)
            {
                /* make all visible now */
                make_all_visible(got_map);
            }
        }
    }

    for(dir = 0; dir < 4; dir++)
    {
        _adj = get_isla_adj(edgy, dir);
        if(_adj != NULL && visited[get_isla_name(_adj) - 1] == FALSE)
        {
            DFS_engine(_adj, visited, got_map, bridge_stack, FALSE); /* New recursion level */
        }
    }

    return;
}

void remove_bridge(bridge *got_bridge)
{
    isla *aux_isla = NULL;
    int i     = 0;
    int dir   = 0;
    int point = 0;

    for( point = 0; point < 2; point++ )
    {
        aux_isla = get_points(got_bridge, point);
        inc_isla_bridge_s_avb(aux_isla);
        for( dir = 0; dir < 4; dir++ )
        {
            for( i = 0; i < 2; i++ )
            {
                if( get_isla_used_bridge(aux_isla, dir, i) == got_bridge )
                {
                    set_isla_used_bridge(aux_isla, dir, i, NULL);
                }
            }
        }
    }

    return;
}

bool DFS_ignition(stack *new_stack, map *got_map, list *isla_list, isla *aux_isla, bool *visited, size_t total_bridges)
{
    int mode = get_map_mode(got_map);
    bool is_solved = FALSE;
    bool erase_invisibility = TRUE;

    if( visited == NULL )
        memory_error("Unable to allocate visited vector");

    #ifdef HEAVY_DEBUG
    printf("Going into isla %d \n", get_isla_name(aux_isla));
    #endif

    DFS_engine(aux_isla, visited, got_map, new_stack, erase_invisibility);
    set_isla_dfs_status(aux_isla, 1); /* Increment DFS status of isla */
    memset(visited, FALSE, sizeof(bool) * (get_n_islas(got_map)));  /*Reset visited array to FALSE*/

    if(get_stack_size(new_stack) == total_bridges)
    {
        if(mode == 1 || mode == 2)
        {
            is_solved = TRUE;
        }
        else
            is_solved = check_for_allconnected(isla_list);
    }

    return is_solved;
}

int backtrack(stack *got_stack, list *isla_list, map *got_map, int obvious, size_t total_bridges)
{
    node   *aux_node    = NULL;
    bridge *aux_bridge  = NULL;
    bridge *last_bridge = NULL;
    bool   is_empty     = FALSE;
    bool   is_solved    = FALSE;
    int    mode         = get_map_mode(got_map);
    int    dfs_counter  = 0;
    isla   *aux_isla    = NULL;
    bool   *visited     = (bool *) calloc(get_n_islas(got_map), sizeof(bool));

    aux_isla = is_map_connectable(isla_list, got_stack, got_map);
    DFS_ignition(got_stack, got_map, isla_list, aux_isla, visited, total_bridges);

    if(get_stack_size(got_stack) == total_bridges)
    {
        if(mode == 1 || mode == 2)
        {
            is_solved = TRUE;
        }
        else
            is_solved = check_for_allconnected(isla_list);
    }


    while(is_empty == FALSE && is_solved == FALSE)
    {
        if(last_bridge != NULL)
        {
            /*make head invisible and not in stack*/
            set_bridge_stackness(get_node_item(get_head(got_stack)), FALSE);
            free(get_bridge_probi_list(last_bridge));
            /* Push head to prohibited list of head->next */
            push_item_to_list(get_bridge_probi_list(last_bridge), get_node_item(get_stack_head(got_stack)));
            define_visibility(get_bridge_probi_list(last_bridge));

            /* Pop stack until last bridge */
            while((bridge *)get_node_item(get_stack_head(got_stack)) != last_bridge) /* Free stack until analysis point */
            {
                aux_node = pop_from_stack(got_stack); /* Pop node from stack */
                aux_bridge = (bridge *) get_node_item(aux_node);
                remove_bridge(aux_bridge);
            }
        }
    
        aux_isla = is_map_connectable(isla_list, got_stack, got_map);
        while (aux_isla != NULL)
        {
            is_solved = DFS_ignition(got_stack, got_map, isla_list, aux_isla, visited, total_bridges); /* DFS remaining points */
            dfs_counter ++;
            sort_list(isla_list, is_isla_greater_avb);
            aux_isla = is_map_connectable(isla_list, got_stack, got_map);
        }
        reset_dfsed_values(isla_list);

        if((int) get_stack_size(got_stack) > obvious+1 && is_solved == FALSE && get_next_node(get_stack_head(got_stack))!= NULL)
        {
            last_bridge = get_node_item(get_next_node(get_stack_head(got_stack)));
        }
        else
        {
            is_empty = TRUE;
        }

    }

    #ifdef DEBUG
    printf(KGRN"Final DFS COUNT: %d\n "KNRM, dfs_counter);
    #endif

    free(visited);
    return define_mode_result(mode, is_solved, isla_list);
}

stack *DFS_manager(list *isla_list, map* got_map)
{
    stack *new_stack   = create_stack();
    int mode = get_map_mode(got_map);
    bool is_solved = FALSE;
    size_t total_bridges = get_numberof_bridges(isla_list);

    if(connect_obvious(new_stack, isla_list, got_map) == FALSE)
    {
        define_mode_result(mode, FALSE, isla_list);
    }

    if(get_stack_size(new_stack) == total_bridges)
    {
        if(mode == 1 || mode == 2)
        {
            is_solved = TRUE;
        }
        else
            is_solved = check_for_allconnected(isla_list);
    }
    #ifdef DEBUG
    print_stack(new_stack, print_bridge);
    #endif

    if(!is_solved)
    {
        sort_list(isla_list, is_isla_greater_avb);
        set_map_mode_result(got_map, backtrack(new_stack, isla_list, got_map, get_stack_size(new_stack), total_bridges));
    }

    free_bridge_vector(got_map);

    return new_stack;
}


