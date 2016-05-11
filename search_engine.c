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
    bridge* new_bridge = (bridge*)get_isla_used_bridge(new_isla, adj_index);

    if(new_bridge != NULL)
        return TRUE;
    else
        return FALSE;
}

/* feels the vector inpath[] if islas are in path */
void create_path_vector(isla *new_isla, bool *inpath)
{
    isla *_adj = NULL;
    int i = 0;

    for(_adj = get_isla_adj(new_isla, i); i < 4; i++)
    {
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
    bool *inpath = (bool*)calloc(get_list_size(isla_list)+1, sizeof(bool));
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

bool is_bridges_available(list *isla_list, list *probi_list)
{
    int n_avb, n_probi;
    node* new_node;

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

bool is_prohibited(isla *victim_isla, int adj_index, list* probi_list)
{
    bridge *victim_bridge, *new_bridge = NULL;
    node* new_node =  NULL;

    new_node = get_head(probi_list);
    victim_bridge = get_isla_used_bridge(victim_isla, adj_index);

    while(new_node != NULL)
    {
        new_bridge = get_node_item(new_node);

        if(new_bridge == victim_bridge)
            return TRUE;

        new_node = get_next_node(new_node);
    }

    return FALSE;
}


bool is_connectable(isla *isla_a, isla *isla_b, int adj_index, map *got_map, list *probi_list)
{
    bridge *new_bridge = NULL;

    if(get_isla_bridge_s_avb(isla_a) > 0 && get_isla_bridge_s_avb(isla_b) > 0)
    {
        new_bridge = (bridge*)get_isla_used_bridge(isla_a, adj_index);
        if(new_bridge != NULL)
        {

            /* Just a little test for now -----------------*/
            if(get_bridges_n_bridges(new_bridge) > 0)
            {
                if(get_points(new_bridge, 0) != isla_a && get_points(new_bridge, 1) != isla_a)
                {
                    printf("\nSOMETHING IS VERY WRONG. CHECK is_connectable\n");
                }
                if(get_points(new_bridge, 0) != isla_b && get_points(new_bridge, 1) != isla_b)
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

        if( probi_list != NULL )
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
    unsigned int i = 0;
    bridge *new_bridge;

    visited[get_isla_name(edgy)] = TRUE;

    /* Travel all nodes, list implementation may be underkill*/
    /* i gives NSEW*/
    for(i = 0; i < 4; i++)
    {
        _adj = get_isla_adj(edgy, i);
        /* Check if exists, check if visited and check if islas are good for connect*/
        if(_adj != NULL && visited[get_isla_name(_adj)] == FALSE && is_connectable(edgy, _adj, i, got_map, probi_list) == TRUE )
        {
            printf("Looking %d , Isla1: %d Isla2: %d ; Available1: %d ; Available2: %d\n",i , get_isla_name(edgy), get_isla_name(_adj), get_isla_bridge_s_avb(edgy), get_isla_bridge_s_avb(_adj));
            new_bridge = connect_islas(edgy, _adj, i);
            push_to_stack(bridge_stack, (item)new_bridge);
        }
    }

    for(i = 0; i < 4; i++)
    {
        _adj = get_isla_adj(edgy, i);
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
    isla *empty = NULL;
    int i = 0;
    int f = 0;

    for( i = 0; i < 2; i++ )
    {
        aux_isla = get_points(got_bridge, i);
        inc_isla_bridge_s_avb(aux_isla);

        for( f = 0; f < 4; f++ )
        {
            if( get_isla_used_bridge(aux_isla, f) == got_bridge && get_bridges_n_bridges(got_bridge) == 1 )
            {
                set_isla_used_bridge(aux_isla, f, empty);
            }
        }

    }

    if( get_bridges_n_bridges(got_bridge) == 2)
    {
        dec_bridge_n_bridges(got_bridge);
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
            memset(visited, FALSE, sizeof(bool) * (get_list_size(isla_list))); /* Reset visited array to FALSE*/
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

void backtrack_engine(bool zeroed, bool stack_empty, map *got_map, stack *got_stack, bridge *last_point, list *isla_list, int mode)
{
    node   *aux_node;
    bridge *aux_bridge;
    isla   *try_isla;
    list   *probi_list;

    printf("Trying to backtack \n");
    /* If we already determined it is zeroed */
    if(zeroed == TRUE)
    {
        return;
    }

    if(stack_empty == TRUE)
    {
        return;
    }

    /* Free prohibition list from head bridge */
    /* free_connected_nodes(get_head(get_bridge_probi_list(get_node_item(get_stack_head(got_stack)))), free_bridge); */
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
    zeroed = check_for_allzero(isla_list);
    if(zeroed == TRUE)
    {
        return;
    }

    if(get_next_node(get_stack_head(got_stack)) == NULL)
    {
        stack_empty = TRUE;
        return;
    }

    backtrack_engine(zeroed, stack_empty, got_map, got_stack, get_node_item(get_next_node(get_stack_head(got_stack))), isla_list, mode);

}

stack *DFS_manager(list *isla_list, int mode, map* got_map)
{
    isla *good_isla  = NULL;
    bool zeroed      = FALSE;
    bool stack_empty = FALSE;
    stack *new_stack = create_stack();

    good_isla = get_isla_for_dfs(isla_list);
    DFS_ignition(new_stack, good_isla, got_map, isla_list, NULL, mode);

    if(mode == 2)
    {
        /* Check if all connected*/
    }

    zeroed = check_for_allzero(isla_list);
    backtrack_engine(zeroed, stack_empty, got_map, new_stack, get_node_item(get_next_node(get_stack_head(new_stack))), isla_list, mode);

    return new_stack;
}


