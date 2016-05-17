#include "basictechniques.h"

bool loner_neighbour(isla *new_isla, stack *got_stack)
{
    int dir = 0;
    isla *_adj = NULL;
    bridge *new_bridge;

    for(dir=0; dir<4; dir++)
    {
        _adj = get_isla_adj(new_isla, dir);
        if(_adj != NULL)
        {
            if(is_connectable(new_isla, _adj, dir, got_stack) == TRUE)
            {
                new_bridge = connect_islas(new_isla, _adj, dir);
                push_to_stack(got_stack, (item)new_bridge);

                if(get_isla_bridge_s_avb(new_isla) == 1)
                {
                    new_bridge = connect_islas(new_isla, _adj, dir);
                    push_to_stack(got_stack, (item)new_bridge);
                }

            }
            else
                return FALSE;
        }
    }

    return TRUE;
}

bool special_impar_treatment(isla *new_isla, stack *got_stack)
{
    int dir = 0;
    int flag_one = 0;
    isla *_adj = NULL;
    bridge *new_bridge = NULL;

    for(dir=0; dir<4; dir++)
    {
        _adj = get_isla_adj(new_isla, dir);
        if(_adj != NULL)
        {
            if(is_connectable(new_isla, _adj, dir, got_stack) == TRUE)
            {
                new_bridge = connect_islas(new_isla, _adj, dir);
                push_to_stack(got_stack, (item)new_bridge);
            }
            else
                return FALSE;

            if(get_isla_bridge_s_avb(_adj) == 0)
            {
                flag_one = 1;
            }
        }
    }

    if(flag_one)
    {
        for(dir=0; dir<4; dir++)
        {
            _adj = get_isla_adj(new_isla, dir);
            if(_adj != NULL)
            {
                if(is_connectable(new_isla, _adj, dir, got_stack) == TRUE)
                {
                    new_bridge = connect_islas(new_isla, _adj, dir);
                    push_to_stack(got_stack, (item)new_bridge);
                }
                else
                    return FALSE;
            }
        }
    }

    return TRUE;
}

bool n_four(isla *magica, stack *got_stack)
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
            connectable += is_connectable(magica, aux, dir, got_stack);
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

        if(connectable == 2)
        {
            push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, a), a));
            push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, a), a));
            push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, b), b));
            push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, b), b));
            break;
        }
        else
        {
            return FALSE;
        }
    }

    return TRUE;
}

bool n_six(isla *magica, stack *got_stack)
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
            connectable += is_connectable(magica, aux, dir, got_stack);
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

        if(connectable == 3)
        {
            push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, a), a));
            push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, a), a));
            push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, b), b));
            push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, b), b));
            push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, c), c));
            push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, c), c));
            break;
        }
        else
        {
            return FALSE;
        }
    }

    return TRUE;
}

bool n_eight(isla *magica, stack *got_stack)
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
            connectable += is_connectable(magica, aux, dir, got_stack);
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

        if(connectable == 4)
        {
            push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, a), a));
            push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, a), a));
            push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, b), b));
            push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, b), b));
            push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, c), c));
            push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, c), c));
            push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, d), d));
            push_to_stack(got_stack, connect_islas(magica, get_isla_adj(magica, d), d));
            break;
        }
        else
        {
            return FALSE;
        }
    }

    return TRUE;
}

bool in_side_3(isla *new_isla, stack *got_stack)
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

    if(flag_one == 3)
    {
        for(dir=0; dir<4; dir++)
        {
            if(is_connectable(new_isla, _adj, dir, got_stack) == TRUE)
            {
                new_bridge = connect_islas(new_isla, _adj, dir);
                push_to_stack(got_stack, (item)new_bridge);
            }
            else
                return FALSE;
        }
    }

    return TRUE;
}

void basic_numeric_connections(isla *new_isla, stack *got_stack)
{

}

bool basic_connections_ok(isla *new_isla, stack *got_stack)
{
    int n_still_bridges = get_isla_bridge_s_avb(new_isla);
    int n_bridges = get_isla_bridges_avb(new_isla);
    int n_adj = get_adj_number(new_isla);
    bool connections_ok = FALSE;

    if(n_adj == 1)
        connections_ok = loner_neighbour(new_isla, got_stack);

    if(n_still_bridges == 2 && n_adj == 3)
    {
        connections_ok = in_side_3(new_isla, got_stack);
    }

    if(((n_still_bridges == 3 && n_adj == 2) || (n_still_bridges == 5 && n_adj == 3) || (n_still_bridges == 7 && n_adj == 4)) && (n_still_bridges == n_bridges))
    {
        connections_ok = special_impar_treatment(new_isla, got_stack);
    }

    if((n_still_bridges == 4 && n_adj == 2) && n_still_bridges == n_bridges)
    {
        connections_ok = n_four(new_isla, got_stack);
    }

    if((n_still_bridges == 6 && n_adj == 3) && n_still_bridges == n_bridges)
    {
        connections_ok = n_six(new_isla, got_stack);
    }

    if((n_still_bridges == 8 && n_adj == 4) && n_still_bridges == n_bridges)
    {
        connections_ok = n_eight(new_isla, got_stack);
    }

    return connections_ok;
}

bool connect_obvious(stack *got_stack, list *isla_list)
{
    node *new_node = NULL;
    isla *new_isla = NULL;
    bool connections_ok = FALSE;

    new_node = get_head(isla_list);

    while(new_node != NULL)
    {
        new_isla = get_node_item(new_node);

        connections_ok = basic_connections_ok(new_isla, got_stack);

        new_node = get_next_node(new_node);
    }

    return connections_ok;
}
