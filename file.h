#ifndef FILE_HEADER
#define FILE_HEADER

#include "map.h"
#include "lists.h"

int read_line(FILE* map_file, int *lineData);
void setup_isla(isla* isla_item, map* map_item, int *lineData, list *list_item);
FILE *change_file_ext(char* original_name);
void print_output_per_map(map *got_map, FILE *outfile, int mode, int mode_result);
void print_created_map();

#endif
