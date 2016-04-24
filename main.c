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

    if(argc != 2 || strcmp(get_filename_ext(argv[1]), "map"))
        file_error("Missing arguments or wrong extension specified on file input");

    map_file = fopen(argv[1], "r");
    if(map_file == NULL)
        file_error("Unable to open file specified");

    /* test to creation and assignment of map */
    read_line(map_file, lineData);
    the_map = create_map();
    the_map = assign_to_map(the_map, lineData);

    fclose(map_file); /* -will be closed when end is reached. also when return is 0. to define later! */
    free_map(the_map);

    return 0;
}
