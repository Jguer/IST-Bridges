#include "map.h"

struct _map {
    isla ***tile;
    int  x_max;
    int  y_max;
    int  n_islas;
    char map_mode;
    int  mode_result;
    node **probi_vector;
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
    new_map->mode_result = NO_SOL;
    new_map->probi_vector = (node **) calloc(((n_islas) * (n_islas + 1))/2 , sizeof(node *));

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

node *get_probi_head(map *got_map, int isla_a, int isla_b)
{
    int index;
    int list_size = got_map->n_islas;

    isla_a = isla_a - 1;
    isla_b = isla_b - 1;

    if (isla_a <= isla_b)
        index = isla_a * list_size - (isla_a - 1) * isla_a / 2 + isla_b - isla_a;
    else
        index = isla_b * list_size - (isla_b - 1) * isla_b / 2 + isla_a - isla_b;

    return got_map->probi_vector[index];
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
    got_map->tile[get_x(get_isla_pos(isla_to_add)) - 1][ get_y(get_isla_pos(isla_to_add)) - 1] = isla_to_add;
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

int get_map_mode_result(map* got_map)
{
    return got_map->mode_result;
}

void set_map_mode_result(map *got_map, int _mode_result)
{
    got_map->mode_result = _mode_result;
    return;
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
                print_pos(get_isla_pos(to_print));
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
                printf("%d ", get_isla_name(to_print));
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
    int xi;

    /* Go lenghtwise*/
    for (xi = 0; xi < get_x_max(got_map); xi ++)
    {
        /* If Cuba is present spread democracy through object destruction */
        free(got_map->tile[xi]);
    }

    free(got_map->tile);
    free(got_map);

    return;
}
