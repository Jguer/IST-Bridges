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

	if(argc != 2 || strcmp(get_filename_ext(argv[1]), "map"))
		file_error("Missing arguments or wrong extension specified on file input");

	map_file = fopen(argv[1], "r");
	if(map_file == NULL)
		file_error("Unable to open file specified");

	fclose(map_file);

    return 0;
}
