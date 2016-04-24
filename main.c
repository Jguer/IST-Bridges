/**************************************
* Bridges @ AED-Project 2016
* by
* João Guerreiro & Mariana Martins
*
* main.c
***************************************/
#include "file.h"
#include "isla.h"
#include "defs.h"
#include "lists.h"
#include "map.h"
#include "bridge.h"

int main(int argc, char **argv)
{
    FILE *map_file;
    int lineData[4];
 	isla* new_isla;
 	list* isla_list;

    if(argc != 2 || strcmp(get_filename_ext(argv[1]), "map"))
        file_error("Missing arguments or wrong extension specified on file input");

    map_file = fopen(argv[1], "r");
    if(map_file == NULL)
        file_error("Unable to open file specified");

    while(read_line(map_file, lineData) != 0)
    {
    	/*Create new map*/

    	isla_list = create_list();
    	/*Read its new islas*/
    	while(read_line(map_file, lineData) == 1)
    	{
    		/*Read isla, create struct isla, anda matrix, line by line*/
    		new_isla = setup_isla(lineData, new_isla);
    		push_item_to_list(isla_list, new_isla);

    	}

    	/*Play the game*/
    	/*free stuff, start over*/
    	free_map(the_map);
    }

    fclose(map_file);
    return 0;
}
