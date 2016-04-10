#include "filereader.h"

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

/* Assigns lineData[] values to any other important values */
void assign_values(int *linedata, int *val0, int *val1, int *val2, int *val3)
{
    *val0 = linedata[0];
    *val1 = linedata[1];
    *val2 = linedata[2];
    *val3 = linedata[3];
    return;
}


