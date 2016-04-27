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
    if(NULL == new_map ) {
        memory_error("Unable to allocate map");
    }

    new_map->x_max= x_max;
    new_map->y_max= y_max;
    new_map->n_bridges = n_bridges;
    new_map->map_mode = map_mode;

    /* Allocate x axis so we can have a nice tile[x] */
    new_map->tile = (isla ***) calloc(x_max, sizeof(isla**));
    if(NULL == new_map->tile)
    {
        memory_error("Unable to allocate x axis of map");
    }
    /* Allocate y axis so we can have a nice tile[x][y] */
    for (i = 0; i < x_max; i++)
    {
        new_map->tile[i] = (isla **) calloc(y_max, sizeof(isla*));
        if(NULL == new_map->tile[i] )
        {
            memory_error("Unable to allocate y axis of map");
        }
    }

    return new_map;
}

isla *get_tile(map *got_map, int x, int y)
{
    /* Base for work is 1-max but base for backend is 0-max-1
     * Example: (1,1)->(0,0)*/
    return got_map->tile[x - 1][y - 1];
}

void set_tile(map *got_map, isla* isla_to_add)
{
    /* Once again position is shifted north-west by 1 */
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
        fprintf(DEBUG_LOC, KYEL "Row %d:\n" KNRM, i);
        j = 0;

        while(j < x_max)
        {
            new = got_map->tile[i][j];
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

    /* Go lenghtwise*/
    for (xi = 0; xi < get_x_max(got_map); xi ++)
    {

        /* Go heightwise*/
        for (yi = 0; yi < get_y_max(got_map); yi ++)
        {
            /* If Cuba is present spread democracy through object destruction */
            if(got_map->tile[xi][yi] != NULL) {
            }
        }

        free(got_map->tile[xi]);
    }

    free(got_map->tile);
    free(got_map);

    return;
}
