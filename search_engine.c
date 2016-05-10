#include "search_engine.h"

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
            return FALSE;

        new_node = get_next_node(new_node);
    }

    return TRUE;
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

        if(is_prohibited(isla_a, adj_index, probi_list))
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

            new_bridge = get_isla_used_bridge(edgy, i);
            if(new_bridge != NULL) /* If already bridge structure is linked in isla_struct*/
            {
                increment_bridges_n_bridges(get_isla_used_bridge(edgy,i));
                /* Push to stack new bridge */
                push_to_stack(bridge_stack, (item)new_bridge);
            }
            else /* Create new link for isla_struct*/
            {
                new_bridge = create_bridge(edgy, _adj, 1);
                set_isla_used_bridge(edgy, i, new_bridge);
                set_isla_used_bridge(_adj, get_opposite_dir(i), new_bridge);
                /* Push to stack new bridge */
                push_to_stack(bridge_stack, (item)new_bridge);
            }
            /* Augment new brigde structure count*/

            dec_isla_bridge_s_avb(edgy);
            dec_isla_bridge_s_avb(_adj);
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

stack *DFS_manager(list *isla_list, int mode, map* got_map)
{
    isla *good_isla;
    bool *visited = (bool *) calloc(get_list_size(isla_list) + 1, sizeof(bool));
    stack *new_stack = create_stack();
    list *probi_list = create_list();

    if(mode == 1 || mode == 2) /* Connect all of them, doesn't matter if grouped or path*/
    {
        good_isla = get_isla_for_dfs(isla_list);
        while(good_isla != NULL ) {
            printf("Going into isla %d \n", get_isla_name(good_isla));
            DFS_engine(good_isla, visited, got_map, new_stack, probi_list);
            set_isla_dfs_status(good_isla, get_isla_dfs_status(good_isla) + 1); /* Increment DFS status of isla */

            memset(visited, FALSE, sizeof(bool) * (get_list_size(isla_list))); /* Reset visited array to FALSE*/
            good_isla = get_isla_for_dfs(isla_list); /* Get new isla for analysis*/
        }
    }
    else if(mode == 3) /* Connect all of them, forcebly a path */
    {
        DFS_engine(get_node_item(get_head(isla_list)), visited, got_map, new_stack, probi_list);
    }
    else /* Invalid Mode */
    {
        fprintf(stderr, KYEL "Good Job, you officially failed at map making. " KRED " Invalid mode\n"KNRM);
    }

    if(mode == 2)
    {
        /* Check if all connected*/
    }

    free_list(probi_list, already_free);
    free(visited);
    return new_stack;
}




