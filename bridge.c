#include "bridge.h"

struct _bridge {
    isla *point[2];
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

    return new_bridge;
}



isla *get_points(bridge *got_bridge, int index)
{
    return got_bridge->point[index];
}

list *get_bridge_probi_list(bridge *got_bridge)
{
    return got_bridge->probi_list;
}

void print_bridge(item got_item)
{
    bridge* got_bridge = (bridge *)got_item;
    fprintf(DEBUG_LOC,
            KMAG "Isla 1 name:" RESET " %d, "
            KMAG "Isla 2 name:" RESET " %d, ",
            get_isla_name(get_points(got_bridge, 0)), get_isla_name(get_points(got_bridge, 1)));

    return;
}

/* Just to know if there is a bridge in an isla direction */
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
    free(got_bridge->probi_list);
    free(got_bridge);

    return;
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

/* checks out for crossed bridges */
bool crossed_fire(isla* isla_a, isla* isla_b, map* got_map)
{
    int a_x, a_y, b_x, b_y;
    int max_x, max_y, min_x, min_y;

    a_x = get_x(get_isla_pos(isla_a));
    a_y = get_y(get_isla_pos(isla_a));
    b_x = get_x(get_isla_pos(isla_b));
    b_y = get_y(get_isla_pos(isla_b));

    /* Two islas in a line */
    if(a_y == b_y)
    {
        if(a_x > b_x)
        {
            max_x = a_x;
            min_x = b_x;
        }
        else
        {
            max_x = b_x;
            min_x = a_x;
        }
        min_x++;

        while(min_x < max_x)
        {
            if(search_for_bridge_inCol(got_map, a_y, min_x) != NULL)
                return TRUE;
            min_x++;
        }
        return FALSE;
    }

    /* Two islas in a column */
    else
    {
        if(a_y > b_y)
        {
            max_y = a_y;
            min_y = b_y;
        }
        else
        {
            max_y = b_y;
            min_y = a_y;
        }
        min_y++;

        while(min_y < max_y)
        {
            if(search_for_bridge_inLine(got_map, a_x, min_y) != NULL)
                return TRUE;
            min_y++;
        }
        return FALSE;
    }
}

/* It searches for a bridge in a line of the map */
bridge* search_for_bridge_inLine(map* got_map, int isla_x, int static_y)
{
    isla *isla_found, *adj_found;
    bridge* bridge_found = NULL;
    int x_adj;

    isla_found = find_next_isla_x(got_map, 1, static_y, isla_x);

    if(isla_found != NULL)
    {
        adj_found =  get_isla_adj(isla_found, 2);
        if(adj_found != NULL)
        {
            x_adj = get_x(get_isla_pos(adj_found));
            if(x_adj > isla_x)
            {
                bridge_found = is_bridge(isla_found, 2);
                if(bridge_found != NULL)
                {
                    return bridge_found;
                }
                else
                    return NULL;
            }
            else
                return NULL;
        }
        else
            return NULL;
    }
    else
        return NULL;
}

/* It searches for a bridge in a column of the map */
bridge* search_for_bridge_inCol(map* got_map, int isla_y, int static_x)
{
    isla *isla_found, *adj_found;
    bridge* bridge_found = NULL;
    int y_adj;

    isla_found = find_next_isla_y(got_map, 1, static_x, isla_y);

    if(isla_found != NULL)
    {
        adj_found =  get_isla_adj(isla_found, 1);
        if(adj_found != NULL)
        {
            y_adj = get_y(get_isla_pos(adj_found));
            if(y_adj > isla_y)
            {
                bridge_found = is_bridge(isla_found, 1);
                if(bridge_found != NULL)
                {
                    return bridge_found;
                }
                else
                    return NULL;
            }
            else
                return NULL;
        }
        else
            return NULL;
    }
    else
        return NULL;
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
/*
stack *gen_essential_bridges(list *isla_list)
{
    stack *initial_stack = NULL;
    int n_bridges, n_adj;
    int i = 0;

    while(new_node != NULL)
    {
        new_isla = get_node_item(new_node);
        n_adj = get_adj_number(new_isla);
        n_bridges = get_isla_bridges_avb(new_isla);

        if(n_adj < n_bridges)
        {
            for(i=0; i<4; i++)
            {
                _adj = get_isla_adj(new_isla, i);
                if(_adj != NULL)
                    connect_islas();
            }
        }
        else if(n_adj == n_bridges)
        {
        }

        new_node = get_next_node(new_node);
    }

    return initial_stack;
}
*/
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
