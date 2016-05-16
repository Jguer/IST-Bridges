/**************************************
* Bridges @ AED-Project 2016
* by
* João Guerreiro & Mariana Martins
*
* main.c
***************************************/

#include "file.h"

int main(int argc, char **argv)
{
    FILE *map_file, *output_file;
    int lineData[4];
    isla *new_isla;
    list *isla_list;
    map *active_map;
    stack *got_stack;
    bool fuck_up = FALSE;

    if(argc != 2 || strcmp(get_filename_ext(argv[1]), "map"))
        file_error("Missing arguments or wrong extension specified on file input");

    map_file = fopen(argv[1], "r");
    if(map_file == NULL)
        file_error("Unable to open file specified");

    output_file = change_file_ext(argv[1]);

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

#ifdef DEBUG
        /*tester: print map, print list*/
        print_list(isla_list, print_isla);
        printf("\n");
        print_map(active_map);
        print_map_graphic(active_map);
        printf("\n");
        /* end of test */
#endif

        find_adj(active_map);
#ifdef DEBUG
        print_adj(isla_list);
#endif
        fuck_up = initial_fuck_up(isla_list);

        /* verify if initial fuck up, if yes: rage quit */
        if(fuck_up != TRUE)
        {
            /*Play the game*/
            got_stack = DFS_manager(isla_list, active_map);
#ifdef _DEBUG
            print_stack(got_stack, print_bridge);
#endif
        }

        /*Write in output file*/
        print_output_per_map(active_map, output_file, isla_list);

        /*free stuff, start over*/
        if(fuck_up != TRUE)
            free_stack(got_stack, already_free);
        free_list(isla_list, free_isla);
        free_map(active_map);
    }

    fclose(map_file);
    fclose(output_file);

    exit(EXIT_SUCCESS);
}
