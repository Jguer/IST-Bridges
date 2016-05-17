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

bool check_for_allzero(list *isla_list)
{
    isla *new_isla;
    node *new_node;

    new_node = get_head(isla_list);

    while(new_node != NULL)
    {
        new_isla = get_node_item(new_node);
        if(get_isla_bridge_s_avb(new_isla) != 0)
            return FALSE;

        new_node = get_next_node(new_node);
    }
    return TRUE;
}

bool is_connected(isla* new_isla, int adj_index)
{
    bridge* new_bridge;
    int i = 0;

    for( i = 0; i < 2; i++)
    {
        new_bridge = (bridge*)get_isla_used_bridge(new_isla, adj_index, i);

        if(new_bridge != NULL)
            return TRUE;
    }

    return FALSE;
}

/* fills the vector inpath[] if islas are in path */
void create_path_vector(isla *new_isla, bool *inpath, bool *visited)
{
    isla *_adj = NULL;
    int i = 0;

    visited[get_isla_name(new_isla)] = TRUE;

    for(i = 0 ; i < 4; i++)
    {
        _adj = get_isla_adj(new_isla, i);
        /* Check if exists and check if visited*/
        if(is_connected(new_isla, i) == TRUE)
        {
            inpath[get_isla_name(new_isla)] = TRUE;
        }
        if(_adj != NULL && visited[get_isla_name(_adj)] == FALSE)
        {
            create_path_vector(_adj, inpath, visited); /* New recursion level */
        }
    }
    return;
}

/* checks for all connected in path */
bool check_for_allconnected(list *isla_list)
{
    isla *new_isla = NULL;
    node *new_node = NULL;
    bool *inpath = (bool*)calloc(get_list_size(isla_list)+1, sizeof(bool)); /*check if +1 need be*/
    bool *visited = (bool*)calloc(get_list_size(isla_list)+1, sizeof(bool));
    int index = 1;

    new_node = get_head(isla_list);
    new_isla = get_node_item(new_node);

    create_path_vector(new_isla, inpath, visited);

    while(index < (int) get_list_size(isla_list)+1)
    {
        if(inpath[index] == FALSE)
        {
            free(visited);
            free(inpath);
            return FALSE;
        }
        index++;
    }
    free(visited);
    free(inpath);
    return TRUE;
}

bool is_bridges_available(list *isla_list)
{
    int n_avb      = 0;
    int n_probi    = 0;
    node* new_node = NULL;

    n_avb = get_numberof_bridges_avb(isla_list);

    while(new_node != NULL)
    {
        n_probi++;
        new_node = get_next_node(new_node);
    }

    if(n_probi < n_avb)
    {
        return TRUE;
    }
    else
        return FALSE;
}

bool is_prohibited(isla *victim_isla, int dir, list* probi_list)
{
    bridge *aux_bridge    = NULL;
    node   *aux_node      = NULL;
    isla   *_adj          = NULL;

    aux_node = get_head(probi_list);
    _adj = get_isla_adj(victim_isla, dir);

    while(aux_node != NULL)
    {
        aux_bridge = get_node_item(aux_node);

        if(get_points(aux_bridge, 0) == victim_isla && get_points(aux_bridge, 1) == _adj)
            return TRUE;
        else if(get_points(aux_bridge, 1) == victim_isla && get_points(aux_bridge, 0) == _adj)
            return TRUE;

        aux_node = get_next_node(aux_node);

    }
    return FALSE;
}


bool is_connectable(isla *isla_a, isla *isla_b, int adj_index, stack *got_stack)
{
    list *probi_list = NULL;

    if(get_stack_head(got_stack) != NULL)
    {
        probi_list = get_bridge_probi_list(get_node_item(get_stack_head(got_stack)));
    }

    if(get_isla_bridge_s_avb(isla_a) > 0 && get_isla_bridge_s_avb(isla_b) > 0)
    {
        if(get_isla_n_bridges(isla_a, adj_index) >= 2)
            return FALSE;

        if(crossed_fire(isla_a, isla_b, got_stack))
            return FALSE;

        if(probi_list != NULL)
        {
            if(is_prohibited(isla_a, adj_index, probi_list))
                return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

int gen_essential_bridges(list *isla_list, stack *got_stack)
{
    int n_bridges, n_adj;
    int dir = 0;
    bridge *new_bridge = NULL;
    node *new_node = NULL;
    isla *new_isla = NULL, *_adj = NULL;

    new_node = get_head(isla_list);

    while(new_node != NULL)
    {
        new_isla = get_node_item(new_node);
        n_adj = get_adj_number(new_isla);
        n_bridges = get_isla_bridges_avb(new_isla);


        if(n_adj < n_bridges)
        {
            for(dir=0; dir<4; dir++)
            {
                _adj = get_isla_adj(new_isla, dir);
                if(_adj != NULL)
                {
                    #ifdef DEBUG
                    printf("Isla1: %d Isla2: %d\n", get_isla_name(new_isla), get_isla_name(_adj));
                    #endif
                    if(is_connectable(new_isla, _adj, dir, got_stack) == TRUE  &&
                        is_connected(new_isla, dir) == FALSE && is_connected(_adj, get_opposite_dir(dir)) == FALSE)
                    {
                        new_bridge = connect_islas(new_isla, _adj, dir);
                        push_to_stack(got_stack, (item)new_bridge);

                        if(is_connectable(new_isla, _adj, dir, got_stack) == TRUE && n_bridges > 2*n_adj)
                        {
                            new_bridge = connect_islas(new_isla, _adj, dir);
                            push_to_stack(got_stack, (item)new_bridge);
                        }
                    }
                }
            }
        }

        new_node = get_next_node(new_node);
    }

    return (int) get_stack_size(got_stack);
}

int gen_dynamic_obivous_bridges(list *isla_list, stack *got_stack, map *got_map)
{
    int n_bridges, n_adj;
    int dir = 0;
    bridge *new_bridge = NULL;
    node *new_node = NULL;
    isla *new_isla = NULL, *_adj = NULL;
    bool is_solved = FALSE;
    int mode = get_map_mode(got_map);

    new_node = get_head(isla_list);

    while(new_node != NULL)
    {
        new_isla = get_node_item(new_node);
        n_adj = get_adj_number(new_isla);
        n_bridges = get_isla_bridge_s_avb(new_isla);


        if(n_adj < n_bridges)
        {
            for(dir=0; dir<4; dir++)
            {
                _adj = get_isla_adj(new_isla, dir);
                if(_adj != NULL)
                {
                    #ifdef DEBUG
                    printf("Isla1: %d Isla2: %d\n", get_isla_name(new_isla), get_isla_name(_adj));
                    #endif
                    if(is_connectable(new_isla, _adj, dir, got_stack) == TRUE)
                    {
                        new_bridge = connect_islas(new_isla, _adj, dir);
                        push_to_stack(got_stack, (item)new_bridge);
                        #ifdef DEBUG
                        printf("Connect %d - %d\n", get_isla_name(new_isla), get_isla_name(_adj));
                        #endif
                    }
                }
            }
        }

        new_node = get_next_node(new_node);
    }

    is_solved = check_for_allzero(isla_list);
    if(mode == 1 || mode == 2)
        return is_solved;

    else
    {
        if(is_solved)
            return check_for_allconnected(isla_list);
        else
            return is_solved;

    }
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

        is_solved = gen_dynamic_obivous_bridges(isla_list, got_stack, got_map);
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
    int   obv_gen      = 0;
    stack *new_stack   = create_stack();
    int mode = get_map_mode(got_map);
    bool is_solved = FALSE;

    obv_gen = gen_essential_bridges(isla_list, new_stack);

    if(mode == 1 || mode == 2)
        is_solved = check_for_allzero(isla_list);
    else if(mode == 3)
        is_solved = check_for_allconnected(isla_list);

    #ifdef DEBUG
    print_stack(new_stack, print_bridge);
    #endif

    is_solved = gen_dynamic_obivous_bridges(isla_list, new_stack, got_map);

    if(!is_solved)
    {
        sort_list(isla_list, is_isla_greater_avb);

        #ifdef DEBUG
        print_list(isla_list, print_isla);
        printf("Number of obvious generated: %d \n", obv_gen);
        #endif

        DFS_ignition(new_stack, got_map, isla_list);

        set_map_mode_result(got_map, backtrack(new_stack, isla_list, got_map, obv_gen + 1));
    }

    return new_stack;
}


