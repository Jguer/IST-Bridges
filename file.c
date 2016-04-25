#include "file.h"

/* Reads the integers from one line of the maps file
 * and stores them in vector lineData[] 
 */
int read_line(FILE* map_file, int *lineData)
{
    int scanfresult;
    int i = 0;

    while(i<4)
    {
        scanfresult = fscanf(map_file, "%d", &lineData[i]);

        /* if end of file: 0 */
        if(scanfresult == EOF)
            return 0;
        /* if end of map: -1 */
        else if(lineData[i] == -1)
            return -1;
        i++;
    }
    return 1;
}

/*  Sets up an island by creating its structure
 *  Points the correspondent position on the map to that structure
 *  Add that isla to the list of islas
 */
void setup_isla(isla* isla_item, map* map_item, int *lineData, list *list_item)
{
    isla_item = create_isla(lineData[0], lineData[1], lineData[2], lineData[3]);
    set_tile(map_item, isla_item);
    push_item_to_list(list_item, isla_item);
    return;
}