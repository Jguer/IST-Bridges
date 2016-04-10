/**************************************
* Bridges @ AED-Project 2016
* by
* João Guerreiro & Mariana Martins
*
* main.c
***************************************/
#include "filereader.h"
#include "isla.h"
#include "defs.h"
#include "lists.h"
#include "map.h"

int main(int argc, char **argv)
{
    FILE *map_file;
    int lineData[4];
    map *the_map; /* only one and can be changed from map to map */
    isla *new_isla;
    isla *new_isla1;
    isla *new_isla2;
    list *new_list;

    if(argc != 2 || strcmp(get_filename_ext(argv[1]), "map"))
        file_error("Missing arguments or wrong extension specified on file input");

    map_file = fopen(argv[1], "r");
    if(map_file == NULL)
        file_error("Unable to open file specified");

    /* test to creation and assignment of map */
    read_line(map_file, lineData);
    the_map = create_map();
    the_map = assign_to_map(the_map, lineData);
    print_map(the_map);

    new_isla = create_isla(1, 4, 6, 2);
    new_isla1 = create_isla(2, 1, 3, 1);
    new_isla2 = create_isla(3, 3, 2, 1);

    new_list = create_list();
    push_item_to_list(new_list, (item)new_isla);
    push_item_to_list(new_list, (item)new_isla1);
    push_item_to_list(new_list, (item)new_isla2);
    print_list(new_list, print_isla);
    free_list(new_list, free_isla);

    fclose(map_file); /* -will be closed when end is reached. also when return is 0. to define later! */
    free_map(the_map);

    return 0;
}
