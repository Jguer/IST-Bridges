#ifndef FILEREADER_HEADER
#define FILEREADER_HEADER

int read_line(FILE* map_file, int *lineData);
void assign_values(int *linedata, int *val0, int *val1, int *val2, int *val3);

#endif