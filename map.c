#include "map.h"
#include "isla.h"

struct _map {
    isla ***tile;
    int x_max;
    int y_max;
    int n_islas;
    char map_mode;
};

map *create_map(int x_max, int y_max, int n_islas, int map_mode)
{
    int i;
    map *new_map;

    new_map = (map *)malloc(sizeof(map));
    if(NULL == new_map ) {
        memory_error("Unable to allocate map");
    }

    new_map->x_max= x_max;
    new_map->y_max= y_max;
    new_map->n_islas = n_islas;
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

int get_n_islas(map* got_map)
{
    return got_map->n_islas;
}

int get_x_max(map* got_map)
{
    return got_map->x_max;
}

int get_y_max(map* got_map)
{
    return got_map->y_max;
}

char get_map_mode(map* got_map)
{
    return got_map->map_mode;
}

void print_map(map* got_map)
{
    int xi = 0, yi = 0;

    isla* to_print;
    printf("Map Print:\n");

    while(xi < get_x_max(got_map))
    {
        fprintf(DEBUG_LOC, KYEL "Row %d:\n" KNRM, xi);
        yi = 0;

        while(yi < get_y_max(got_map))
        {
            to_print = got_map->tile[xi][yi];
            if(to_print != NULL)
                print_pos(get_pos_isla(to_print));
            yi++;
        }
        printf("\n");

        xi++;
    }

    return;
}

void print_map_graphic(map* got_map)
{
    int xi = 0, yi = 0;

    isla* to_print;

    printf("Print map (Graphical):\n");
    while(yi < get_y_max(got_map))
    {
        xi = 0;

        while(xi < get_x_max(got_map))
        {
            to_print = got_map->tile[xi][yi];
            if(to_print != NULL)
            {
                printf("%d ", get_name_isla(to_print));
            }
            else
            {
                printf("0 ");
            }
            xi++;
        }
        printf("\n");

        yi++;
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
