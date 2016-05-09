#include "bridge.h"

struct _bridge {
    isla *point[2];
    int n_bridges;
};

bridge *create_bridge(isla *isla_eins, isla *isla_zwei)
{
    bridge *new_bridge;

    new_bridge = (bridge *)malloc(sizeof(bridge));

    new_bridge->point[0] = isla_eins;
    new_bridge->point[1] = isla_zwei;
    new_bridge->n_bridges = 0;

    return new_bridge;
}

isla *get_points(bridge *got_bridge, int index)
{
    return got_bridge->point[index];
}

int get_bridges_n_bridges(bridge* got_bridge)
{
    return got_bridge->n_bridges;
}

void free_bridge(bridge *got_bridge)
{
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
    static_x = get_x(get_pos_isla(active_row_node));                /* gets which column we are working on */

    y = get_y(get_pos_isla(new));                                   /* gets the row from which we are working on */
    new_next = find_next_isla_y(got_map, static_x, y+1, y_max);     /* gets the next isla in that column */
    if(new_next != NULL)                                            /* if an isla is actually found, it's an adjacent */
    {
        set_adj_isla(new, new_next, 1);
        set_adj_isla(new_next, new, 0);
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
            y = get_y(get_pos_isla(new));
            x = get_x(get_pos_isla(new));
            new_next = find_next_isla_x(got_map, x+1, y, x_max);
            if(new_next != NULL)                                    /* if the row has more islas */
            {
                set_adj_isla(new, new_next, 2);
                set_adj_isla(new_next, new, 3);
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
        fprintf(DEBUG_LOC, KGRN "Isla: %d - " RESET, get_name_isla(new));
        for(i=0; i<4; i++)
        {
            if(get_adj_isla(new, i) != NULL)
            {
                fprintf(DEBUG_LOC, KBLU "Adj %d:" RESET KGRN "%d " KNRM, i, get_name_isla(get_adj_isla(new, i)));
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

    a_x = get_x(get_pos_isla(isla_a));
    a_y = get_y(get_pos_isla(isla_a));
    b_x = get_x(get_pos_isla(isla_b));
    b_y = get_y(get_pos_isla(isla_b));

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

    isla_found = find_next_isla_x(got_map, 0, static_y, isla_x);

    if(isla_found != NULL)
    {
        adj_found =  get_adj_isla(isla_found, 2);
        if(adj_found != NULL)
        {
            x_adj = get_x(get_pos_isla(adj_found));
            if(x_adj > isla_x)
            {
                bridge_found = get_used_bridge(isla_found, 2);
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

    isla_found = find_next_isla_y(got_map, 0, static_x, isla_y);

    if(isla_found != NULL)
    {
        adj_found =  get_adj_isla(isla_found, 2);
        if(adj_found != NULL)
        {
            y_adj = get_y(get_pos_isla(adj_found));
            if(y_adj > isla_y)
            {
                bridge_found = get_used_bridge(isla_found, 2);
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