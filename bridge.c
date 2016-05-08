#include "bridge.h"

struct _bridge {
    isla *point[2];
};

bridge *create_bridge(isla *isla_eins, isla *isla_zwei)
{
    bridge *new_bridge;

    new_bridge = (bridge *)malloc(sizeof(bridge));

    new_bridge->point[0] = isla_eins;
    new_bridge->point[1] = isla_zwei;

    return new_bridge;
}

isla **get_points(bridge *got_bridge)
{
    return got_bridge->point;
}

void free_bridge(bridge *got_bridge)
{
    free(bridge);

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

