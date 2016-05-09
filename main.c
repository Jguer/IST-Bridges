/**************************************
* Bridges @ AED-Project 2016
* by
* João Guerreiro & Mariana Martins
*
* main.c
***************************************/

#include "bridge.h"
#include "search_engine.h"

int main(int argc, char **argv)
{
    FILE *map_file;
    int lineData[4];
    isla *new_isla;
    list *isla_list;
    map *active_map;
    stack *got_stack;

    if(argc != 2 || strcmp(get_filename_ext(argv[1]), "map"))
        file_error("Missing arguments or wrong extension specified on file input");

    map_file = fopen(argv[1], "r");
    if(map_file == NULL)
        file_error("Unable to open file specified");

    while(read_line(map_file, lineData) != 0)
    {
        /*Create new map*/
        active_map = create_map(lineData[1], lineData[0], lineData[2], lineData[3]);
        isla_list = create_list();

        /*Read isla, create struct isla, add to the map matrix, line by line*/
        while(read_line(map_file, lineData) == 1)
        {
            new_isla = NULL;
            setup_isla(new_isla, active_map, lineData, isla_list);
        }

        /*tester: print map, print list*/
        print_list(isla_list, print_isla);
        printf("\n");
        print_map(active_map);
        print_map_graphic(active_map);
        printf("\n");
        /* end of test */

        find_adj(active_map);
        print_adj(isla_list);

        /*Play the game*/
        got_stack = DFS_manager(isla_list, 1, active_map);

        /*free stuff, start over*/
        free_list(isla_list, free_isla);
        free_map(active_map);
    }

    fclose(map_file);
    return 0;
}
