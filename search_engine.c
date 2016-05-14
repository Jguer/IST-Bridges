#include "search_engine.h"

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

/* feels the vector inpath[] if islas are in path */
void create_path_vector(isla *new_isla, bool *inpath)
{
    isla *_adj = NULL;
    int i = 0;

    for(i = 0 ; i < 4; i++)
    {
        _adj = get_isla_adj(new_isla, i);
        /* Check if exists and check if visited*/
        if(_adj != NULL && inpath[get_isla_name(_adj)] == FALSE && is_connected(new_isla, i) == TRUE)
        {
            inpath[get_isla_name(_adj)] = TRUE;
            create_path_vector(_adj, inpath); /* New recursion level */
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
    int index = 0;

    new_node = get_head(isla_list);
    new_isla = get_node_item(new_node);

    create_path_vector(new_isla, inpath);

    while(index < (int) get_list_size(isla_list)+1)
    {
        if(inpath[index] == FALSE)
            return FALSE;
        index++;
    }

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


bool is_connectable(isla *isla_a, isla *isla_b, int adj_index, stack *got_stack, list *probi_list)
{
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

void DFS_engine(isla *edgy, bool *visited, map* got_map, stack *bridge_stack, list *probi_list)
{
    isla *_adj = NULL;
    unsigned int dir = 0;
    bridge *new_bridge;

    visited[get_isla_name(edgy)] = TRUE;

    /* Travel all nodes, list implementation may be underkill*/
    /* i gives NSEW*/
    for(dir = 0; dir < 4; dir++)
    {
        _adj = get_isla_adj(edgy, dir);
        /* Check if exists, check if visited and check if islas are good for connect*/
        if(_adj != NULL && visited[get_isla_name(_adj)] == FALSE && is_connectable(edgy, _adj, dir, bridge_stack, probi_list) == TRUE )
        {
            printf("Looking %d , Isla1: %d Isla2: %d ; Available1: %d ; Available2: %d\n", dir , get_isla_name(edgy), get_isla_name(_adj), get_isla_bridge_s_avb(edgy), get_isla_bridge_s_avb(_adj));
            new_bridge = connect_islas(edgy, _adj, dir);
            push_to_stack(bridge_stack, (item)new_bridge);
        }
    }

    for(dir = 0; dir < 4; dir++)
    {
        _adj = get_isla_adj(edgy, dir);
        if(_adj != NULL && visited[get_isla_name(_adj)] == FALSE)
        {
            DFS_engine(_adj, visited, got_map, bridge_stack, probi_list); /* New recursion level */
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

void DFS_ignition(stack *new_stack, isla *first_isla, map *got_map, list *isla_list, list *probi_list, int mode)
{
    isla *aux_isla = first_isla;
    bool *visited  = (bool *) calloc(get_list_size(isla_list) + 1, sizeof(bool));

    if( visited == NULL )
        memory_error("Unable to allocate visited vector");

    if(mode == 1 || mode == 2) /* Connect all of them, doesn't matter if grouped or path*/
    {
        while(aux_isla != NULL ) {
            printf("Going into isla %d \n", get_isla_name(aux_isla));
            DFS_engine(aux_isla, visited, got_map, new_stack, probi_list);
            set_isla_dfs_status(aux_isla, get_isla_dfs_status(aux_isla) + 1); /* Increment DFS status of isla */
            memset(visited, FALSE, sizeof(bool) * (get_list_size(isla_list)));  /*Reset visited array to FALSE*/
            aux_isla = get_isla_for_dfs(isla_list); /* Get new isla for analysis*/
        }
    }
    else if(mode == 3) /* Connect all of them, forcebly a path */
    {
        DFS_engine(aux_isla, visited, got_map, new_stack, probi_list);
    }
    else /* Invalid Mode */
    {
        fprintf(stderr, KYEL "Good Job, you officially failed at map making. " KRED " Invalid mode\n"KNRM);
    }

    reset_dfsed_values(isla_list);
    free(visited);
    return;
}


int backtrack(stack *got_stack, list *isla_list, map *got_map, int mode)
{
    node   *aux_node    = NULL;
    bridge *aux_bridge  = NULL;
    bridge *last_bridge = NULL;
    list   *probi_list  = NULL;
    bool   is_connected = FALSE;
    bool   is_empty     = is_stack_empty(got_stack);
    bool   is_solved    = check_for_allzero(isla_list);

    printf("Is empty? %d Is solved? %d \n", is_empty, is_solved);
    last_bridge = get_node_item(get_next_node(get_stack_head(got_stack))); /* We know backtrack starts from next node, so get first bridge */

    while( is_empty == FALSE && is_solved == FALSE)
    {
        printf("Is empty? %d Is solved? %d \n", is_empty, is_solved);
        printf("Last Point: %d-%d \n", get_isla_name(get_points(last_bridge, 0)), get_isla_name(get_points(last_bridge, 1)));
        printf("To remove : %d-%d \n", get_isla_name(get_points(get_node_item(get_head(got_stack)), 0)), get_isla_name(get_points(get_node_item(get_head(got_stack)), 1)));
        printf("Trying to backtack. Last stack \n");
        print_stack(got_stack, print_bridge);

        /* Free prohibition list from head bridge */
        free_connected_nodes(get_head(get_bridge_probi_list(get_node_item(get_stack_head(got_stack)))), free_bridge);
        /* Push head to prohibited list of head->next */
        push_item_to_list(get_bridge_probi_list(last_bridge), get_node_item(get_stack_head(got_stack)));

        while( (bridge *)get_node_item(get_stack_head(got_stack)) != last_bridge) /* Free stack until analysis point */
        {
            aux_node = pop_from_stack(got_stack); /* Pop node from stack */
            aux_bridge = (bridge *) get_node_item(aux_node);

            remove_bridge(aux_bridge);
            free_node(aux_node, already_free);
        }

        probi_list = get_bridge_probi_list(last_bridge);
        DFS_ignition(got_stack, get_isla_for_dfs(isla_list), got_map, isla_list, probi_list, mode);

        is_solved = check_for_allzero(isla_list); /* That did not check out, so let us check for all zero on map*/

        if(get_stack_size(got_stack) > 1)
        {
            last_bridge = get_node_item(get_next_node(get_stack_head(got_stack)));
        }
        else
        {
            is_empty = TRUE;
        }
    }

    is_connected = FALSE; /* Check for connection here*/


    return 1;
}

int backtrack_engine(bool zeroed, bool stack_empty, map *got_map, stack *got_stack, bridge *last_point, list *isla_list, int mode)
{
    node   *aux_node;
    bridge *aux_bridge;
    isla   *try_isla;
    list   *probi_list;

    printf("Last Point: %d-%d \n", get_isla_name(get_points(last_point, 0)), get_isla_name(get_points(last_point, 1)));
    printf("To remove : %d-%d \n", get_isla_name(get_points(get_node_item(get_head(got_stack)), 0)), get_isla_name(get_points(get_node_item(get_head(got_stack)), 1)));
    printf("Trying to backtack. Last stack \n");
    print_stack(got_stack, print_bridge);
    /* If we already determined it is zeroed */
    if(zeroed == TRUE)
    {
        return GOT_SOL;
    }

    if(stack_empty == TRUE)
    {
        return NO_SOL;
    }

    /* Free prohibition list from head bridge */
    free_connected_nodes(get_head(get_bridge_probi_list(get_node_item(get_stack_head(got_stack)))), free_bridge);
    /* Push head to prohibited list of head->next */
    push_item_to_list(get_bridge_probi_list(last_point), get_node_item(get_stack_head(got_stack)));

    /* Bummer, that did not work. Get over depression and get on re(cursing) */
    /* Pop stack until last point */
    while( (bridge *)get_node_item(get_stack_head(got_stack)) != last_point )
    {
        aux_node = pop_from_stack(got_stack); /* Pop node from stack */
        aux_bridge = (bridge *) get_node_item(aux_node);

        remove_bridge(aux_bridge);
        free_node(aux_node, already_free);
    }

    probi_list = get_bridge_probi_list(get_node_item(get_stack_head(got_stack)));

    try_isla = get_isla_for_dfs(isla_list);
    DFS_ignition(got_stack, try_isla, got_map, isla_list, probi_list, mode);

    /* That did not check out, so let us check for all zero on map*/
    if(check_for_allzero(isla_list) == TRUE)
    {
        zeroed = TRUE;
        free_connected_nodes(get_head(get_bridge_probi_list(last_point)), free_bridge);
        return GOT_SOL;
    }

    if(get_next_node(get_stack_head(got_stack)) == NULL)
    {
        free_connected_nodes(get_head(get_bridge_probi_list(last_point)), free_bridge);
        stack_empty = TRUE;
        return NO_SOL;
    }

    backtrack_engine(zeroed, stack_empty, got_map, got_stack, get_node_item(get_next_node(get_stack_head(got_stack))), isla_list, mode);

    return 0;
}

stack *DFS_manager(list *isla_list, int mode, map* got_map)
{
    isla *good_isla  = NULL;
    stack *new_stack = create_stack();

    good_isla = get_isla_for_dfs(isla_list);
    DFS_ignition(new_stack, good_isla, got_map, isla_list, NULL, mode);

    backtrack(new_stack, isla_list, got_map, mode);

    return new_stack;
}

stack *gen_essential_bridges(list *isla_list)
{
    stack *initial_stack = NULL;
    int n_bridges, n_adj;
    int dir = 0;
    bridge *new_bridge = NULL;
    node *new_node = NULL;
    isla *new_isla = NULL, *_adj = NULL;

    initial_stack = create_stack();
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
                    if(is_connected(new_isla, dir) == FALSE && is_connected(_adj, get_opposite_dir(dir)) == FALSE)
                    {
                        new_bridge = connect_islas(new_isla, _adj, dir);
                        push_to_stack(initial_stack, (item)new_bridge);
                    }
                }
            }
        }

        new_node = get_next_node(new_node);
    }

    return initial_stack;
}
