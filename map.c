#include "map.h"
#include "isla.h"

struct _map {
    isla ***tile;
    int x_max;
    int y_max;
    int n_bridges;
    char map_mode;
};

map *create_map(int x_max, int y_max, int n_bridges, int map_mode)
{
    int i;
    map *new_map;

    new_map = (map *)malloc(sizeof(map));
    if( NULL == new_map ) {
        memory_error("Unable to allocate map");
    }

    new_map->x_max= x_max;
    new_map->y_max= y_max;
    new_map->n_bridges = n_bridges;
    new_map->map_mode = map_mode;

    /* Allocate x axis so we can have a nice tile[x] */
    new_map->tile = (isla ***) calloc(x_max , sizeof(isla**));
    if( NULL == new_map->tile)
    {
        memory_error("Unable to allocae x axis of map");
    }
    /* Allocate y axis so we can have a nice tile[x] */
    for ( i = 0; i < x_max; i++)
    {
        new_map->tile[i] = (isla **) calloc(y_max , sizeof(isla*));
        if( NULL == new_map->tile[i] )
        {
            memory_error("Unable to allocate y axis of map");
        }
    }

    return new_map;
}

isla *get_tile(map *got_map, int x, int y)
{
    x --;
    y --;
    return got_map->tile[x][y];
}

void set_tile(map *got_map, isla* isla_to_add)
{
    got_map->tile[get_x(get_pos_isla(isla_to_add)) - 1][ get_y(get_pos_isla(isla_to_add)) - 1] = isla_to_add;
    return;
}

int get_n_bridges(map* got_map)
{
    return got_map->n_bridges;
}

int get_x_max(map* got_map)
{
    return got_map->x_max;
}

int get_y_max(map* got_map)
{
    return got_map->y_max;
}

void print_map(int x_max, int y_max, map* got_map)
{
    int i = 0, j = 0;
    isla* new;

    while(i < y_max)
    {
            fprintf(DEBUG_LOC, KYEL "Row %d:\n" RESET, i);
            j = 0;

        while(j < x_max)
        {
            new = get_tile(got_map, j, i);
            if(new != NULL)
                print_pos(get_pos_isla(new));
            j++;
        }
        printf("\n");

        i++;
    }

    return;
}

void free_map(item got_item)
{
    map *got_map = (map *)got_item;
    int xi, yi;

    for (xi = 0; xi < get_x_max(got_map); xi ++)
    {
        for (yi = 0; yi < get_y_max(got_map); yi ++)
        {
            if( get_tile(got_map, xi, yi) != 0) {
                free_isla((item)get_tile(got_map, xi, yi));
            }
        }
    }

    free(got_map);

    return;
}
