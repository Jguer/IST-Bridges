#ifndef FILE_HEADER
#define FILE_HEADER

#include "map.h"
#include "lists.h"

int read_line(FILE* map_file, int *lineData);
void setup_isla(isla* isla_item, map* map_item, int *lineData, list *list_item);

#endif
