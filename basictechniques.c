#include "basictechniques.h"

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


bool is_prohibited(isla *a, isla *b, map *got_map)
{
    bridge *new_bridge = NULL;

    new_bridge = get_bridge_from_vector(got_map, get_isla_name(a), get_isla_name(b), 0);
    if(new_bridge == NULL)
        return FALSE;
    else
    {
        if(get_bridge_stackness(new_bridge) == FALSE)
        {   
            if(get_bridge_visibility(new_bridge) == TRUE)
                return FALSE;
            else
                return TRUE;
        }
        else
        {
            new_bridge = get_bridge_from_vector(got_map, get_isla_name(a), get_isla_name(b), 1);
            if(new_bridge == NULL)
                return FALSE;
            if(get_bridge_stackness(new_bridge) == FALSE)
            {   
                if(get_bridge_visibility(new_bridge) == TRUE)
                    return FALSE;
                else
                    return TRUE;
            }

        }  
    }
    return TRUE;    
}

bool is_connectable(isla *isla_a, isla *isla_b, int adj_index, stack *got_stack, map *got_map)
{
    if(get_isla_bridge_s_avb(isla_a) > 0 && get_isla_bridge_s_avb(isla_b) > 0)
    {
        if(get_isla_n_bridges(isla_a, adj_index) >= 2)
            return FALSE;

        if(crossed_fire(isla_a, isla_b, got_stack))
            return FALSE;

        if(is_prohibited(isla_a, isla_b, got_map))
            return FALSE;
    }
    return TRUE;
}

bool four_in_side(isla *new_isla, stack *got_stack, map *got_map)
{
    int dir = 0;
    isla *_adj;
    int flag_one = 0;
    bridge *new_bridge = NULL;

    for(dir = 0; dir < 4; dir ++)
    {
        _adj = get_isla_adj(new_isla, dir);
        if (_adj != NULL)
        {
             if(get_isla_bridge_s_avb(_adj) == 1)
                flag_one++;
        }
    }

    if(flag_one == 2)
    {
        for(dir = 0; dir < 4; dir ++)
        {
            _adj = get_isla_adj(new_isla, dir);
            if (_adj != NULL)
            {
                if(is_connectable(new_isla, _adj, dir, got_stack, got_map) && get_isla_bridge_s_avb(_adj) == 1)
                {
                    new_bridge = connect_islas(new_isla, _adj, dir, got_map);
                    push_to_stack(got_stack, (item)new_bridge);
                }
                else if(is_connectable(new_isla, _adj, dir, got_stack, got_map) && get_isla_bridge_s_avb(_adj) > 1)
                {
                    new_bridge = connect_islas(new_isla, _adj, dir, got_map);
                    push_to_stack(got_stack, (item)new_bridge);
                    new_bridge = connect_islas(new_isla, _adj, dir, got_map);
                    push_to_stack(got_stack, (item)new_bridge);
                }
                else
                    return FALSE;

            }
        }
    }

    return TRUE;
 
}

bool loner_neighbour(isla *new_isla, stack *got_stack, map* got_map)
{
    int dir = 0;
    isla *_adj = NULL;
    bridge *new_bridge;

    for(dir=0; dir<4; dir++)
    {
        _adj = get_isla_adj(new_isla, dir);
        if(_adj != NULL)
        {
            if(is_connectable(new_isla, _adj, dir, got_stack, got_map) == TRUE)
            {
                #ifdef BT_DEBUG
                printf("Adding Bridge at Loner Neighbour\n");
                #endif
                new_bridge = connect_islas(new_isla, _adj, dir, got_map);
                push_to_stack(got_stack, (item)new_bridge);

                if(get_isla_bridge_s_avb(new_isla) == 1)
                {
                    new_bridge = connect_islas(new_isla, _adj, dir, got_map);
                    push_to_stack(got_stack, (item)new_bridge);
                }
            }
            else
                return FALSE;
        }
    }

    return TRUE;
}

bool special_impar_treatment(isla *new_isla, stack *got_stack, int n_adj, map *got_map)
{
    int dir = 0;
    int flag_one = 0;
    isla *_adj = NULL;
    bridge *new_bridge = NULL;
    int connectable = 0;

    for(dir=0; dir<4; dir++)
    {
        _adj = get_isla_adj(new_isla, dir);
        if(_adj != NULL)
        {
            if(is_connectable(new_isla, _adj, dir, got_stack, got_map) == TRUE)
            {
                connectable++;
            }
        }
    }

    if(connectable == n_adj)
    {
        for(dir=0; dir<4; dir++)
        {
            _adj = get_isla_adj(new_isla, dir);
            if(_adj != NULL)
            {
                printf("%p\n", get_bridge_from_vector_index(got_map, 30, 1));
                new_bridge = connect_islas(new_isla, _adj   , dir, got_map);
                push_to_stack(got_stack, (item)new_bridge);

                if(get_isla_bridge_s_avb(_adj) == 0)
                {
                    flag_one = 1;
                }
            }
        }
    }
    else
        return FALSE;

    if(flag_one)
    {
        for(dir=0; dir<4; dir++)
        {
            _adj = get_isla_adj(new_isla, dir);
            if(_adj != NULL)
            {
                if(get_isla_bridge_s_avb(_adj) != 0)
                {
                    new_bridge = connect_islas(new_isla, _adj, dir, got_map);
                    push_to_stack(got_stack, (item)new_bridge);
                }
            }
        }
    }

    return TRUE;
}

bool n_four(isla *magica, stack *got_stack, map *got_map)
{
    int dir = 0;
    isla *aux;
    int connectable = 0;
    int a = 5;
    int b = 5;

    for( dir = 0; dir < 4; dir ++)
    {
        aux = get_isla_adj( magica, dir);
        if (aux != NULL)
        {
            connectable += is_connectable(magica, aux, dir, got_stack, got_map);
            if(a == 5)
            {
                a = dir;
                if(get_isla_bridge_s_avb(get_isla_adj(magica, a)) < 2)
                {
                    return FALSE;
                }
            }
            else
            {
                b = dir;
                if(get_isla_bridge_s_avb(get_isla_adj(magica, b)) < 2)
                {
                    return FALSE;
                }
            }
        }
    }

    if(connectable == 2)
    {
        #ifdef BT_DEBUG
        printf("Adding Bridge at n_four\n");
        #endif
        push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, a), a, got_map));
        push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, a), a, got_map));
        push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, b), b, got_map));
        push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, b), b, got_map));
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

bool n_six(isla *magica, stack *got_stack, map *got_map)
{
    int dir = 0;
    isla *aux;
    int connectable = 0;
    int a = 5;
    int b = 5;
    int c = 5;

    for( dir = 0; dir < 4; dir ++)
    {
        aux = get_isla_adj( magica, dir);
        if (aux != NULL)
        {
            connectable += is_connectable(magica, aux, dir, got_stack, got_map);
            if(a == 5)
            {
                a = dir;
                if(get_isla_bridge_s_avb(get_isla_adj(magica, a)) < 2)
                {
                    return FALSE;
                }
            }
            else if(b == 5)
            {
                b = dir;
                if(get_isla_bridge_s_avb(get_isla_adj(magica, b)) < 2)
                {
                    return FALSE;
                }
            }
            else
            {
                c = dir;
                if(get_isla_bridge_s_avb(get_isla_adj(magica, c)) < 2)
                {
                    return FALSE;
                }
            }
        }
    }
    if(connectable == 3)
    {
        #ifdef BT_DEBUG
        printf("Adding Bridge at n_six\n");
        #endif
        push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, a), a, got_map));
        push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, a), a, got_map));
        push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, b), b, got_map));
        push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, b), b, got_map));
        push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, c), c, got_map));
        push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, c), c, got_map));
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

bool n_eight(isla *magica, stack *got_stack, map *got_map)
{
    int dir = 0;
    isla *aux;
    int connectable = 0;
    int a = 5;
    int b = 5;
    int c = 5;
    int d = 5;

    for( dir = 0; dir < 4; dir ++)
    {
        aux = get_isla_adj( magica, dir);
        if (aux != NULL)
        {
            connectable += is_connectable(magica, aux, dir, got_stack, got_map);
            if(a == 5)
            {
                a = dir;
                if(get_isla_bridge_s_avb(get_isla_adj(magica, a)) < 2)
                    return FALSE;
            }
            else if(b == 5)
            {
                b = dir;
                if(get_isla_bridge_s_avb(get_isla_adj(magica, b)) < 2)
                    return FALSE;
            }
            else if(c == 5)
            {
                c = dir;
                if(get_isla_bridge_s_avb(get_isla_adj(magica, c)) < 2)
                    return FALSE;
            }
            else
            {
                d = dir;
                if(get_isla_bridge_s_avb(get_isla_adj(magica, d)) < 2)
                    return FALSE;
            }
        }
    }

    if(connectable == 4)
    {
        #ifdef BT_DEBUG
        printf("Adding Bridge at n_eight\n");
        #endif
        push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, a), a, got_map));
        push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, a), a, got_map));
        push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, b), b, got_map));
        push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, b), b, got_map));
        push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, c), c, got_map));
        push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, c), c, got_map));
        push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, d), d, got_map));
        push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, d), d, got_map));
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

bool in_side_3(isla *new_isla, stack *got_stack, map *got_map)
{
    int flag_one = 0;
    int dir = 0;
    isla *_adj = NULL;
    bridge *new_bridge = NULL;

    for(dir=0; dir<4; dir++)
    {
        _adj = get_isla_adj(new_isla, dir);
        if(_adj != NULL)
        {
            if(get_isla_bridge_s_avb(_adj) == 1 && is_connectable(new_isla, _adj, dir, got_stack, got_map) == TRUE)
            {
                flag_one++;
            }
        }
    }

    if(flag_one == 3)
    {
        for(dir=0; dir<4; dir++)
        {
            _adj = get_isla_adj(new_isla, dir);
            if(_adj != NULL)
            {
                if(is_connectable(new_isla, _adj, dir, got_stack, got_map) == TRUE)
                {
                    new_bridge = connect_islas(new_isla, _adj, dir, got_map);
                    push_to_stack(got_stack, (item)new_bridge);
                }
                else
                    return FALSE;
            }
        }
    }

    return TRUE;
}

bool special_6(isla *new_isla, stack *got_stack, map *got_map)
{
    int dir = 0;
    isla *_adj = NULL;
    bridge *new_bridge = NULL;

    for(dir=0; dir<4; dir++)
    {
        _adj = get_isla_adj(new_isla, dir);
        if(_adj != NULL)
        {
            if(is_connectable(new_isla, _adj, dir, got_stack, got_map) == TRUE && get_isla_bridge_s_avb(_adj) != 1)
            {
                new_bridge = connect_islas(new_isla, _adj, dir, got_map);
                push_to_stack(got_stack, (item)new_bridge);
            }
            else
                return FALSE;
        }
    }

    return TRUE;
}

bool special_2(isla *new_isla, stack *got_stack, map *got_map)
{
    int flag_one = 0;
    int dir = 0;
    isla *_adj = NULL;
    bridge *new_bridge = NULL;

    for(dir=0; dir<4; dir++)
    {
        _adj = get_isla_adj(new_isla, dir);
        if(_adj != NULL)
        {
            if(get_isla_bridge_s_avb(_adj) == 1)
            {
                flag_one++;
            }
        }
    }

    if(flag_one == 2)
    {
        for(dir=0; dir<4; dir++)
        {
            _adj = get_isla_adj(new_isla, dir);
            if(_adj != NULL)
            {
                if(is_connectable(new_isla, _adj, dir, got_stack, got_map) == TRUE)
                {
                    new_bridge = connect_islas(new_isla, _adj, dir, got_map);
                    push_to_stack(got_stack, (item)new_bridge);
                }
                else
                    return FALSE;
            }
        }
    }

    return TRUE;
}

bool basic_connections_ok(isla *new_isla, stack *got_stack, map *got_map)
{
    int n_still_bridges = get_isla_bridge_s_avb(new_isla);
    int n_bridges = get_isla_bridges_avb(new_isla);
    int n_adj = get_adj_number(new_isla);
    bool connections_ok = FALSE;

    printf("%p\n", get_bridge_from_vector_index(got_map, 30, 1));
    if(n_adj == 1)
        connections_ok = loner_neighbour(new_isla, got_stack, got_map);

    if(n_still_bridges == 2 && n_adj == 3)
    {
        connections_ok = in_side_3(new_isla, got_stack, got_map);
    }

    if(n_still_bridges == 2 && n_adj == 2)
    {
        connections_ok = special_2(new_isla, got_stack, got_map);
    }

    if(((n_still_bridges == 3 && n_adj == 2) || (n_still_bridges == 5 && n_adj == 3) || (n_still_bridges == 7 && n_adj == 4)) && (n_still_bridges == n_bridges))
    {
        connections_ok = special_impar_treatment(new_isla, got_stack, n_adj, got_map);
    }
    printf("%p\n", get_bridge_from_vector_index(got_map, 30, 1));

    if((n_still_bridges == 4 && n_adj == 2) && n_still_bridges == n_bridges)
    {
        connections_ok = n_four(new_isla, got_stack, got_map);
    }

    if((n_still_bridges == 6 && n_adj == 3) && n_still_bridges == n_bridges)
    {
        connections_ok = n_six(new_isla, got_stack, got_map);
    }

    if((n_still_bridges == 6 && n_adj == 4) && n_still_bridges == n_bridges)
    {
        connections_ok = special_6(new_isla, got_stack, got_map);
    }

    if((n_still_bridges == 8 && n_adj == 4) && n_still_bridges == n_bridges)
    {
        connections_ok = n_eight(new_isla, got_stack, got_map);
    }

    if((n_still_bridges == 4 && n_adj == 3) && n_still_bridges == n_bridges)
    {
        connections_ok = four_in_side(new_isla, got_stack, got_map);
    }
    return connections_ok;
}

bool connect_obvious(stack *got_stack, list *isla_list, map *got_map)
{
    node *new_node = NULL;
    isla *new_isla = NULL;
    bool connections_ok = FALSE;
    new_node = get_head(isla_list);
    print_bridge_vector(got_map);
    while(new_node != NULL)
    {
        new_isla = get_node_item(new_node);

        connections_ok = basic_connections_ok(new_isla, got_stack, got_map);
        if(!connections_ok)
            return FALSE;

        print_bridge_vector(got_map);
        printf("%p\n", get_bridge_from_vector_index(got_map, 30, 1));
        new_node = get_next_node(new_node);
    }

    return connections_ok;
}

