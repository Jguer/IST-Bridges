#include "bridge.h"

struct _bridge {
    isla *point[2];
    int written;
    list *probi_list;
};

bridge *create_bridge(isla *point_one, isla *point_two)
{
    bridge *new_bridge = NULL;

    new_bridge = (bridge *)malloc(sizeof(bridge));
    if( new_bridge == NULL )
        memory_error("Unable to create structure bridge");

    new_bridge->point[0]   = point_one;
    new_bridge->point[1]   = point_two;
    new_bridge->probi_list = create_list();
    new_bridge->written    = 0;

    return new_bridge;
}

void set_bridge_written(bridge *got_bridge, int value)
{
    got_bridge->written = value;
    return;
}

bool get_bridge_written(bridge *got_bridge)
{
    return got_bridge->written;
}

isla *get_points(bridge *got_bridge, int index)
{
    return got_bridge->point[index];
}

list *get_bridge_probi_list(bridge *got_bridge)
{
    return got_bridge->probi_list;
}

int get_isla_x_from_bridge(bridge *got_bridge, int i)
{
    return get_x(get_isla_pos(get_points(got_bridge, i)));
}

int get_isla_y_from_bridge(bridge *got_bridge, int i)
{
    return get_y(get_isla_pos(get_points(got_bridge, i)));
}


void print_bridge(item got_item)
{
    bridge* got_bridge = (bridge *)got_item;
    fprintf(DEBUG_LOC,
            KMAG "Isla 1 name:" RESET " %d, "
            KMAG "Isla 2 name:" RESET " %d \n",
            get_isla_name(get_points(got_bridge, 0)), get_isla_name(get_points(got_bridge, 1)));

    if(got_bridge->probi_list != NULL)
        print_list(got_bridge->probi_list, print_bridge);
    return;
}

/* Just to know if there is any bridge in an isla direction */
bridge *is_bridge(isla *new_isla, int dir)
{
    bridge *got_bridge = NULL;

    got_bridge = (bridge *)get_isla_used_bridge(new_isla, dir, 0);

    if(got_bridge == NULL)
        got_bridge = get_isla_used_bridge(new_isla, dir, 1);

    return got_bridge;
}

void free_bridge(item got_item)
{
    bridge *got_bridge = (bridge *)got_item;

    if(get_head(got_bridge->probi_list) != NULL)
    {
        free_connected_nodes(get_head(got_bridge->probi_list), free_bridge);
    }

    free(got_bridge->probi_list);
    free(got_bridge);

    return;
}

int get_isla_n_bridges(isla *new_isla, int dir)
{
    int bridge_counter = 0, i = 0;
    bridge *new_bridge = NULL;

    for(i = 0; i < 2; i++)
    {
        new_bridge = (bridge*) get_isla_used_bridge(new_isla, dir, i);
        if(new_bridge != NULL)
        {
            bridge_counter ++;
        }
    }
    return bridge_counter;
}

/*find the next isla on a row*/
isla* find_next_isla_x(map *got_map, int x, int static_y, int x_max)
{
    isla *new = NULL;
    while(new == NULL && x <= x_max)
    {
        new = get_tile(got_map, x, static_y);
        x++;
    }
    return new;
}

/*find the next isla on a column*/
isla* find_next_isla_y(map *got_map, int static_x, int y, int y_max)
{
    isla *new = NULL;
    while(new == NULL && y <= y_max)
    {
        new = get_tile(got_map, static_x, y);
        y++;
    }
    return new;
}

/*sets all adjacent islas in a column*/
void find_adj_y(isla* active_row_node, map *got_map)
{
    isla *new = NULL, *new_next = NULL;
    int y_max, static_x, y;

    new = active_row_node;                                          /* new gets the first isla on the column */
    y_max = get_y_max(got_map);
    static_x = get_x(get_isla_pos(active_row_node));                /* gets which column we are working on */

    y = get_y(get_isla_pos(new));                                   /* gets the row from which we are working on */
    new_next = find_next_isla_y(got_map, static_x, y+1, y_max);     /* gets the next isla in that column */
    if(new_next != NULL)                                            /* if an isla is actually found, it's an adjacent */
    {
        set_isla_adj(new, new_next, 1);
        set_isla_adj(new_next, new, 0);
    }
}

void find_adj(map* got_map)
{
    isla *new = NULL, *new_next = NULL;
    int y = 1, y_max, x = 1, x_max;

    y_max = get_y_max(got_map);
    x_max = get_x_max(got_map);

    while(y <= y_max)                                               /* check till last row */
    {
        new = find_next_isla_x(got_map, 1, y, x_max);
        while(new != NULL)                                          /* check till last cloumn */
        {
            y = get_y(get_isla_pos(new));
            x = get_x(get_isla_pos(new));
            new_next = find_next_isla_x(got_map, x+1, y, x_max);
            if(new_next != NULL)                                    /* if the row has more islas */
            {
                set_isla_adj(new, new_next, 2);
                set_isla_adj(new_next, new, 3);
            }

            find_adj_y(new, got_map);                               /* find islas in  all column now */
            new = new_next;
        }
        y++;
    }
}


void print_adj(list* isla_list)
{
    isla* new;
    node* node;
    int i = 0;

    node = get_head(isla_list);

    while(node != NULL)
    {
        new = get_node_item(node);
        fprintf(DEBUG_LOC, KGRN "Isla: %d - " RESET, get_isla_name(new));
        for(i=0; i<4; i++)
        {
            if(get_isla_adj(new, i) != NULL)
            {
                fprintf(DEBUG_LOC, KBLU "Adj %d:" RESET KGRN "%d " KNRM, i, get_isla_name(get_isla_adj(new, i)));
            }
            else
            {
                fprintf(DEBUG_LOC, KBLU "Adj %d:" RESET KGRN "X " KNRM, i);
            }
        }
        printf("\n");

        node = get_next_node(node);
    }

}

bool is_cross_vertical(int islas_x, int isla_a_y, int isla_b_y, bridge *new_bridge)
{
    int min_isla = 0, max_isla = 0;

    if(get_isla_y_from_bridge(new_bridge, 0) != get_isla_y_from_bridge(new_bridge, 1))
        return FALSE;

    if(isla_a_y > isla_b_y)
    {
        min_isla  = isla_b_y;
        max_isla = isla_a_y;
    }
    else
    {
        min_isla  = isla_a_y;
        max_isla = isla_b_y;
    }

    if((get_isla_x_from_bridge(new_bridge, 0) < islas_x
            && get_isla_x_from_bridge(new_bridge, 1) > islas_x) ||
       (get_isla_x_from_bridge(new_bridge, 1) < islas_x
            && get_isla_x_from_bridge(new_bridge, 0) > islas_x))
    {
        if(min_isla < get_isla_y_from_bridge(new_bridge, 0)
            && get_isla_y_from_bridge(new_bridge, 0) < max_isla)
        {
            return TRUE;
        }
    }

    return FALSE;
}

bool is_cross_horizontal(int islas_y, int isla_a_x, int isla_b_x, bridge *new_bridge)
{
    int min_isla = 0, max_isla = 0;

    if(get_isla_x_from_bridge(new_bridge, 0) != get_isla_x_from_bridge(new_bridge, 1))
        return FALSE;

    if(isla_a_x > isla_b_x)
    {
        min_isla  = isla_b_x;
        max_isla = isla_a_x;
    }
    else
    {
        min_isla  = isla_a_x;
        max_isla = isla_b_x;
    }

    if((get_isla_y_from_bridge(new_bridge, 0) < islas_y
            && get_isla_y_from_bridge(new_bridge, 1) > islas_y) ||
       (get_isla_y_from_bridge(new_bridge, 1) < islas_y
            && get_isla_y_from_bridge(new_bridge, 0) > islas_y))
    {
        if(min_isla < get_isla_x_from_bridge(new_bridge, 0)
            && get_isla_x_from_bridge(new_bridge, 0) < max_isla)
        {
            return TRUE;
        }
    }

    return FALSE;
}

bool crossed_fire(isla* isla_a, isla* isla_b, stack *got_stack)
{
    int a_x, a_y, b_x, b_y;
    bridge *new_bridge =  NULL;
    node *new_node =  NULL;

    new_node = get_stack_head(got_stack);

    while(new_node != NULL)
    {
        new_bridge = get_node_item(new_node);
        a_x = get_x(get_isla_pos(isla_a));
        a_y = get_y(get_isla_pos(isla_a));
        b_x = get_x(get_isla_pos(isla_b));
        b_y = get_y(get_isla_pos(isla_b));

        /* Asking for a vertical bridge possibility */
        if(a_x == b_x)
        {
            if(is_cross_vertical(a_x, a_y, b_y, new_bridge))
                return TRUE;
        }
        else
        {
            if(is_cross_horizontal(a_y, a_x, b_x, new_bridge))
                return TRUE;
        }

        new_node = get_next_node(new_node);
    }
    return FALSE;
}


int get_numberof_bridges(list *isla_list)
{
    isla *new_isla;
    node *new_node;
    int n = 0;

    new_node = get_head(isla_list);

    while(new_node != NULL)
    {
        new_isla = get_node_item(new_node);
        n += get_isla_bridges_avb(new_isla);

        new_node = get_next_node(new_node);
    }

    return n/2;
}

int get_numberof_bridges_avb(list *isla_list)
{
    isla *new_isla;
    node *new_node;
    int n = 0;

    new_node = get_head(isla_list);

    while(new_node != NULL)
    {
        new_isla = get_node_item(new_node);
        n += get_isla_bridge_s_avb(new_isla);

        new_node = get_next_node(new_node);
    }

    return n/2;
}

void free_isla(item got_item)
{
    isla *got_isla = (isla *)got_item;
    isla *adj_isla = NULL;
    bridge *active_bridge;
    int dir = 0, index = 0;

    /* O say can you see
     * by the dawn's early light*/
    free_pos(get_isla_pos(got_isla));

    for(dir = 0; dir < 4; dir++)
    {
        index = 0;
        while(index <= 1)
        {
            active_bridge = get_isla_used_bridge(got_isla, dir, index);
            if(active_bridge != NULL)
            {
                adj_isla = get_points(active_bridge, 0);
                if(get_isla_name(adj_isla) == get_isla_name(got_isla))
                {
                    adj_isla = get_points(active_bridge, 1);
                }

                set_isla_used_bridge(adj_isla, get_opposite_dir(dir), index, NULL);
                free_bridge(active_bridge);
            }
            index++;
        }
    }

    /* Bring democracy to the pointer */
    free(got_isla);

    return;
}

bridge *connect_islas(isla *isla_a, isla *isla_b, int dir)
{
    bridge *got_bridge = NULL;

    if(get_isla_used_bridge(isla_a, dir, 0) == NULL)
    {
        got_bridge = create_bridge(isla_a, isla_b);
        set_isla_used_bridge(isla_a, dir, 0, (bridge *)got_bridge);
        set_isla_used_bridge(isla_b, get_opposite_dir(dir), 0, (bridge *)got_bridge);
    }
    else if(get_isla_used_bridge(isla_a, dir, 1) == NULL)
    {
        got_bridge = create_bridge(isla_a, isla_b);
        set_isla_used_bridge(isla_a, dir, 1, (bridge *)got_bridge);
        set_isla_used_bridge(isla_b, get_opposite_dir(dir), 1, (bridge *)got_bridge);
    }

    /* Decrease available connections in islas */
    dec_isla_bridge_s_avb(isla_a);
    dec_isla_bridge_s_avb(isla_b);

    return got_bridge;
}

/* A function that detects if the map is already fucked up or not */
bool initial_fuck_up(list *isla_list)
{
    isla *new_isla, *_adj;
    node *new_node;
    int n_total_bridges = 0, i = 0;

    new_node = get_head(isla_list);

    while(new_node != NULL)
    {
        new_isla = get_node_item(new_node);

        /*
         * check if the sum of bridges from adj islas are less
         * than the number of bridges of the isla being tested
         */
        for(i = 0; i < 4; i++)
        {
            _adj = get_isla_adj(new_isla, i);

            if(_adj != NULL)
            {
                if(get_isla_bridges_avb(_adj) >= 2)
                    n_total_bridges += 2;
                else if(get_isla_bridges_avb(_adj) == 1)
                    n_total_bridges += 1;
            }
        }

        /* if there aren't enough bridges for an isla */
        if(n_total_bridges < get_isla_bridges_avb(new_isla))
            return TRUE;

        n_total_bridges = 0; i = 0;

        new_node = get_next_node(new_node);
    }
    return FALSE;
}

bool is_impar(int n)
{
    if(n == 3 || n == 5 || n == 7)
        return TRUE;
    else
        return FALSE;
}

bool n_four(isla *magica, stack *got_stack)
{
    int dir = 0;
    isla *aux;
    int connectable;
    int a = 5;
    int b = 5;

    if(get_adj_number(magica) == 2 && get_isla_bridge_s_avb(magica) == 4)
    {
        for( dir = 0; dir < 4; dir ++)
        {
            aux = get_isla_adj( magica, i);
            if (aux != NULL)
            {
                connectable += is_connectable(magica, aux, i, got_stack);
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
    }

    return TRUE;
}

bool n_four(isla *magica, stack *got_stack)
{
    int dir = 0;
    isla *aux;
    int connectable;
    int a = 5;
    int b = 5;

    if(get_adj_number(magica) == 2 && get_isla_bridge_s_avb(magica) == 4)
    {
        for( dir = 0; dir < 4; dir ++)
        {
            aux = get_isla_adj( magica, i);
            if (aux != NULL)
            {
                connectable += is_connectable(magica, aux, i, got_stack);
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
    }

    return TRUE;
}

bool n_six(isla *magica, stack *got_stack)
{
    int dir = 0;
    isla *aux;
    int connectable;
    int a = 5;
    int b = 5;
    int c = 5;

    if(get_adj_number(magica) == 2 && get_isla_bridge_s_avb(magica) == 6)
    {
        for( dir = 0; dir < 4; dir ++)
        {
            aux = get_isla_adj( magica, i);
            if (aux != NULL)
            {
                connectable += is_connectable(magica, aux, i, got_stack);
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
    }

    return TRUE;
}

bool n_eight(isla *magica, stack *got_stack)
{
    int dir = 0;
    isla *aux;
    int connectable;
    int a = 5;
    int b = 5;
    int c = 5;
    int d = 5;

    if(get_adj_number(magica) == 2 && get_isla_bridge_s_avb(magica) == 8)
    {
        for( dir = 0; dir < 4; dir ++)
        {
            aux = get_isla_adj( magica, i);
            if (aux != NULL)
            {
                connectable += is_connectable(magica, aux, i, got_stack);
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
    }

    return TRUE;
}

void basic_numeric_connections(isla *new_isla, stack *got_stack)
{

 

}

void connect_obvious(map *got_map, stack *got_stack, list *isla_list)
{
    node *new_node = NULL;
    isla *new_isla = NULL;
    bridge *new_bridge = NULL;

    new_node = get_head(isla_list);

    while(new_node != NULL)
    {
        new_isla = get_node_item(new_node);

        basic_numeric_connections(new_isla, got_stack);

        new_node = get_next_node(new_node);
    }
}
