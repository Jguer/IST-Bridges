#include "search_engine.h"

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
            if(get_map_mode(got_map) && visited[get_isla_name(_adj) - 1] == TRUE )
            {
                continue;
            }

            #ifdef HEAVY_DEBUG
            printf("Looking %d , Isla1: %d Isla2: %d ; Available1: %d ; Available2: %d\n", dir , get_isla_name(edgy), get_isla_name(_adj), get_isla_bridge_s_avb(edgy), get_isla_bridge_s_avb(_adj));
            #endif
            /* Create new bridge and push it to stack */
            push_to_stack(bridge_stack, connect_islas(edgy, _adj, dir);
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

bool DFS_ignition(stack *new_stack, map *got_map, list *isla_list)
{
    isla *aux_isla = NULL;
    bool *visited  = (bool *) calloc(get_n_islas(got_map), sizeof(bool));
    int  mode      = get_map_mode(got_map);

    if( visited == NULL )
        memory_error("Unable to allocate visited vector");

    if(mode == 1 || mode == 2) /* Connect all of them, doesn't matter if grouped or path*/
    {
        aux_isla = get_isla_for_dfs(isla_list);
        while(aux_isla != NULL)
        {
            #ifdef HEAVY_DEBUG
            printf("Going into isla %d \n", get_isla_name(aux_isla));
            #endif
            DFS_engine(aux_isla, visited, got_map, new_stack);
            set_isla_dfs_status(aux_isla, get_isla_dfs_status(aux_isla) + 1); /* Increment DFS status of isla */
            memset(visited, FALSE, sizeof(bool) * (get_n_islas(got_map)));  /*Reset visited array to FALSE*/
            sort_list(isla_list, is_isla_greater_avb);
            aux_isla = get_isla_for_dfs(isla_list); /* Get new isla for analysis*/
        }
        reset_dfsed_values(isla_list);
        free(visited);
        return check_for_allzero(isla_list);
    }
    else if(mode == 3) /* Connect all of them, forcebly a path */
    {
        aux_isla = get_node_item(get_head(isla_list));
        /* Run 2 DFS engines to make sure path is generated*/
        DFS_engine(aux_isla, visited, got_map, new_stack);
        memset(visited, FALSE, sizeof(bool) * (get_n_islas(got_map))); /*Reset visited array to FALSE*/
        DFS_engine(aux_isla, visited, got_map, new_stack);
        reset_dfsed_values(isla_list);
        free(visited);
        return check_for_allconnected(isla_list);
    }
    else /* Invalid Mode */
    {
        fprintf(stderr, KYEL "Good Job, you officially failed at map making. " KRED " Invalid mode\n"KNRM);
        exit(0);
    }
}

int backtrack(stack *got_stack, list *isla_list, map *got_map, int obvious)
{
    node   *aux_node    = NULL;
    bridge *aux_bridge  = NULL;
    bridge *last_bridge = NULL;
    bool   is_empty     = is_stack_empty(got_stack);
    bool   is_solved    = FALSE;
    int    mode         = get_map_mode(got_map);
    int    dfs_counter  = 0;

    last_bridge = get_node_item(get_next_node(get_stack_head(got_stack))); /* We know backtrack starts from next node, so get first bridge */

    if(mode == 1 || mode == 2)
        is_solved = check_for_allzero(isla_list);
    else if(mode == 3)
        is_solved = check_for_allconnected(isla_list);
    else
        fprintf(stderr, KYEL "Good Job, you officially failed at map making. " KRED " Invalid mode\n"KNRM);

    while(is_empty == FALSE && is_solved == FALSE)
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

        is_solved = DFS_ignition(got_stack, got_map, isla_list); /* DFS remaining points */
        dfs_counter ++;

        if((int) get_stack_size(got_stack) >= obvious && is_solved == FALSE && get_next_node(get_stack_head(got_stack))!= NULL)
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
        printf("Number of obvious generated: %d \n", obv_gen);
        #endif

        DFS_ignition(new_stack, got_map, isla_list);

        set_map_mode_result(got_map, backtrack(new_stack, isla_list, got_map, obv_number + 1));
    }

    return new_stack;
}


