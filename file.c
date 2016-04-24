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
        if(scanfresult == NULL)
            return 0;
        /* if end of map: -1 */
        else if(lineData[i] == -1)
            return -1;
        i++;
    }
    return 1;
}


isla *setup_isla(int *lineData, isla *new)
{
	new = create_isla(lineData[0], lineData[1], lineData[2], lineData[3]);

	return new;
}


