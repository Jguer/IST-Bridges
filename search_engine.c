#include "search_engine.h"

isla *is_map_connectable(list *isla_list, stack *got_stack)
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
                if(is_connectable(new_isla, _adj, dir, got_stack) == TRUE && get_isla_dfs_status(new_isla) == 0)
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


void DFS_engine(isla *edgy, bool *visited, map* got_map, stack *bridge_stack)
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
        if(_adj != NULL && is_connectable(edgy, _adj, dir, bridge_stack) == TRUE )
        {
            if(get_map_mode(got_map) == 3 && visited[get_isla_name(_adj) - 1] == TRUE )
            {
                continue;
            }
            #ifdef HEAVY_DEBUG
            printf("Looking %d , Isla1: %d Isla2: %d ; Available1: %d ; Available2: %d\n", dir , get_isla_name(edgy), get_isla_name(_adj), get_isla_bridge_s_avb(edgy), get_isla_bridge_s_avb(_adj));
            #endif

            /* Create new bridge and push it to stack */
            push_to_stack(bridge_stack, connect_islas(edgy, _adj, dir));
        }


    }

    for(dir = 0; dir < 4; dir++)
    {
        _adj = get_isla_adj(edgy, dir);
        if(_adj != NULL && visited[get_isla_name(_adj) - 1] == FALSE)
        {
            DFS_engine(_adj, visited, got_map, bridge_stack); /* New recursion level */
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

bool DFS_ignition(stack *new_stack, map *got_map, list *isla_list, isla *aux_isla)
{
    bool *visited  = (bool *) calloc(get_n_islas(got_map), sizeof(bool));
    int  mode      = get_map_mode(got_map);

    if( visited == NULL )
        memory_error("Unable to allocate visited vector");

    if( aux_isla != NULL )
    {
        #ifdef HEAVY_DEBUG
        printf("Going into isla %d \n", get_isla_name(aux_isla));
        #endif
        DFS_engine(aux_isla, visited, got_map, new_stack);
        set_isla_dfs_status(aux_isla, 1); /* Increment DFS status of isla */
        memset(visited, FALSE, sizeof(bool) * (get_n_islas(got_map)));  /*Reset visited array to FALSE*/
        free(visited);
        if( mode == 3 )
        {
            return check_for_allconnected(isla_list);
        }
        else
        {
            return check_for_allzero(isla_list);
        }
    }
    else
    {
        return FALSE;
    }
}

int backtrack(stack *got_stack, list *isla_list, map *got_map, int obvious)
{
    node   *aux_node    = NULL;
    bridge *aux_bridge  = NULL;
    bridge *last_bridge = NULL;
    bool   is_empty     = FALSE;
    bool   is_solved    = FALSE;
    int    mode         = get_map_mode(got_map);
    int    dfs_counter  = 0;
    isla *aux_isla = NULL;

    is_solved = check_for_allzero(isla_list);
    if(mode == 2 && is_solved == TRUE)
        is_solved = check_for_allconnected(isla_list);

    while(is_empty == FALSE && is_solved == FALSE)
    {
        if( last_bridge != NULL )
        {
            #ifdef HEAVY_DEBUG
            printf("Last Point: %d-%d \n", get_isla_name(get_points(last_bridge, 0)), get_isla_name(get_points(last_bridge, 1)));
            printf("To remove : %d-%d \n", get_isla_name(get_points(get_node_item(get_head(got_stack)), 0)), get_isla_name(get_points(get_node_item(get_head(got_stack)), 1)));
            printf("Trying to backtack. Last stack \n");
            print_stack(got_stack, print_bridge);
            #endif

            /* free_connected_nodes(get_head(get_bridge_probi_list(get_node_item(get_stack_head(got_stack)))), free_bridge); */
            merge_lists(get_bridge_probi_list(last_bridge), get_bridge_probi_list(get_node_item(get_stack_head(got_stack))));
            /* Push head to prohibited list of head->next */
            push_item_to_list(get_bridge_probi_list(last_bridge), get_node_item(get_stack_head(got_stack)));
            /* Pop stack until last bridge */
            while((bridge *)get_node_item(get_stack_head(got_stack)) != last_bridge) /* Free stack until analysis point */
            {
                aux_node = pop_from_stack(got_stack); /* Pop node from stack */
                aux_bridge = (bridge *) get_node_item(aux_node);

                remove_bridge(aux_bridge);
                free_node(aux_node, already_free);
            }

            if(!connect_obvious(got_stack, isla_list))
            {
                is_solved = FALSE;
            }
            else
            {
                is_solved = check_for_allzero(isla_list);
                if(mode == 3 && is_solved == TRUE)
                    is_solved = check_for_allconnected(isla_list);
            }
        }


        aux_isla = is_map_connectable(isla_list, got_stack);
        while (aux_isla != NULL)
        {
            is_solved = DFS_ignition(got_stack, got_map, isla_list, aux_isla); /* DFS remaining points */
            dfs_counter ++;
            aux_isla = is_map_connectable(isla_list, got_stack);
        }
        reset_dfsed_values(isla_list);

        if((int) get_stack_size(got_stack) > obvious && is_solved == FALSE && get_next_node(get_stack_head(got_stack))!= NULL)
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

    return define_mode_result(mode, is_solved, isla_list);
}

stack *DFS_manager(list *isla_list, map* got_map)
{
    int   obv_number   = 0;
    stack *new_stack   = create_stack();
    int mode = get_map_mode(got_map);
    bool is_solved = FALSE;


    if(!connect_obvious(new_stack, isla_list))
    {
        define_mode_result(mode, FALSE, isla_list);
    }

    obv_number = get_stack_size(new_stack);

    if(mode == 1 || mode == 2)
        is_solved = check_for_allzero(isla_list);
    else if(mode == 3)
        is_solved = check_for_allconnected(isla_list);

    #ifdef DEBUG
    print_stack(new_stack, print_bridge);
    #endif

    if(!is_solved)
    {
        sort_list(isla_list, is_isla_greater_avb);

        #ifdef DEBUG
        print_list(isla_list, print_isla);
        printf("Number of obvious generated: %d \n", obv_number);
        #endif

        set_map_mode_result(got_map, backtrack(new_stack, isla_list, got_map, obv_number + 1));
    }

    return new_stack;
}


