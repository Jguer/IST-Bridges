#include "map.h"
#include "isla.h"

struct _map {
    isla ***tile;
    int x_max;
    int y_max;
    char map_mode;
};

map *create_map(int x_max, int y_max, int map_mode)
{
    int i;
    map *new_map;

    new_map = (map *)malloc(sizeof(map));
    if( NULL == new_map ) {
        memory_error("Unable to allocate map");
    }

    new_map->x_max= x_max;
    new_map->y_max= y_max;

    /* Allocate x axis so we can have a nice tile[x] */
    new_map->tile = (isla ***) calloc(x_max , sizeof(isla**));
    if( NULL == new_map->tile)
    {
        memory_error("Unable to allocae x axis of map");
    }
    /* Allocate y axis so we can have a nice tile[x] */
    for ( i = 0; i < x_max; i++)
    {
        new_map->tile[i] = (isla **) calloc( y_max , sizeof(isla*));
        if( NULL == new_map->tile[i] )
        {
            memory_error("Unable to allocate y axis of map");
        }
    }



    return new_map;
}

isla *get_tile(map *got_map, int x, int y)
{
    return got_map->tile[x][y];
}

void set_tile(map *got_map, isla *isla_to_add)
{
    got_map->tile[get_x(get_pos_isla(isla_to_add))][ get_y(get_pos_isla(isla_to_add))] = isla_to_add;
    return;
}
