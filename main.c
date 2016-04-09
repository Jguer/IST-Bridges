/**************************************
* Bridges @ AED-Project 2016
* by 
* João Guerreiro & Mariana Martins
* 
* main.c
***************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filereader.h"
#include "defs.h"


int main(int argc, char **argv)
{
	FILE *map_file;
	int lineData[4];

	if(argc != 2 || strcmp(get_filename_ext(argv[1]), "map"))
		file_error("Missing arguments or wrong extension specified on file input");

	map_file = fopen(argv[1], "r");
	if(map_file == NULL)
		file_error("Unable to open file specified");

	read_line(map_file, lineData);
	assign_values(lineData, &line, &col, &n_ilhas, &var);

	fclose(map_file); /* -will be closed when end is reached. also when return is 0. to define later! */

    return 0;
}
