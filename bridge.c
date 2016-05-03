#include "bridge.h"

/* returns numbers in between [0, limit] */
int randomize(int limit)
{
    int generated = 0;
    time_t t;

    srand((unsigned) time(&t));

    return generated = rand()%(limit+1);
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

    while(new != NULL)
    {
        y = get_y(get_pos_isla(new));                               /* gets the row from which we are working on */
        new_next = find_next_isla_y(got_map, static_x, y+1, y_max); /* gets the next isla in that column */
        if(new_next != NULL)                                        /* if an isla is actually found, it's an adjacent */
        {
            set_adj_isla(new, new_next, 1);
            set_adj_isla(new_next, new, 0);
        }
        new = new_next;                                             /* start in the next found isla */
    }
}

void find_adj(map* got_map)
{
    isla *new = NULL, *new_next = NULL;
    int y = 1, y_max, x = 1;

    y_max = get_y_max(got_map);
    new = find_next_isla_x(got_map, 1, 1, y_max);                   /* find the isla closest to map origin */

    while(y <= y_max)                                               /* check till last row */
    {
        while(new != NULL)                                          /* check till last cloumn */
        {
            y = get_y(get_pos_isla(new));
            x = get_x(get_pos_isla(new));
            new_next = find_next_isla_x(got_map, x+1, y, y_max);
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


/*******************************************************
 * Connect random islas funtions:
 * (1) find random node in the list
 * (2) find random isla-adj in isla/node chosen
 * (3) make them friends!
 * (4) return friendship bond in form of bridge struct
 *******************************************************/

/* step (1) */
node* find_random_node(map *got_map, list *isla_list)
{
    int n_rand = 0, aux = 0;
    node *node = NULL;

    n_rand = randomize(get_n_bridges(got_map));

    while(aux < n_rand && node == NULL)
    {
        node = get_head(isla_list);
        node = get_next_node(node);
        aux++;
    }

    return node;
}

/* step (2) */
int find_random_adj(isla* main_isla)
{
    int index = 0;
    isla* adj_isla = NULL;

    while(adj_isla == NULL)
    {
        index = randomize(3);
        adj_isla = get_adj_isla(main_isla, index);
    }

    return index;
}

/* step (3) and (4) */
bridge* connect_islas(map* got_map, list* isla_list)
{
    isla* main_isla = NULL;
    bridge* connected_bridge = NULL;
    int index = 0;

    main_isla = get_node_item(find_random_node(got_map, isla_list));
    index = find_random_adj(main_isla);

    /* set bridge now */

    return connected_bridge;
}

/*********************************
 * end of random isla functions
 *********************************/
