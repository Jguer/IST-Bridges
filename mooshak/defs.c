/*********************************************************************************
*     File Name           :     Projects/bridges-aed/defs.c
*     Created By          :     jguer
*     Creation Date       :     [2016-03-31 15:21]
*     Last Modified       :     [2016-05-10 20:19]
*     Description         :      Useful definitions for project
**********************************************************************************/

#include "defs.h"

struct _pos {
    int x;
    int y;
};

char *get_filename_ext(char *filename)
{
    char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

void file_error(char *msg)
{
    fprintf(stderr, KRED"Error during opening file attempt.\n"KNRM);
    fprintf(stderr, KRED"Msg: %s\n",msg);
    fprintf(stderr, KRED"Exit Program due to unmanaged error.\n"KNRM);

    exit(1);
}

void memory_error(char *msg)
{
    fprintf(stderr, KRED"Error during memory reserve attempt.\n"KNRM);
    fprintf(stderr, KRED"Msg: %s\n",msg);
    fprintf(stderr, KRED"Exit Program due to unmanaged error.\n"KNRM);

    exit(1);
}

/* Assigns vector[] values to any other important values
 * Usage: assign_values(vector, &a, &b, &c, ..., NULL);
*/
void assign_values(int* vector, int* n_args, ...)
{
    va_list ap;
    int* n_arg = n_args;
    int i = 0;

    va_start(ap, n_args);
    n_arg = n_args;

    while(n_arg)
    {
        *n_arg = vector[i];
        n_arg = va_arg(ap, int*);
        ++i;
    }
    va_end(ap);
}


/* POSITION */
pos *create_pos(int x,int y)
{
    pos* new_pos = (pos *) malloc(sizeof(pos));
    if(new_pos == NULL)
    {
        memory_error("Unable to allocate position");
    }

    new_pos->x = x;
    new_pos->y = y;

    return new_pos;
}

int get_x(pos *gotpos)
{
    return gotpos->x;
}

int get_y(pos *gotpos)
{
    return gotpos->y;
}

void free_pos(item pos_to_free)
{
    free((pos *) pos_to_free);
    return;
}

void print_pos(item gotpos)
{
    printf("Position: %d %d\n", get_x((pos *) gotpos), get_y((pos *) gotpos));
    return;
}

int get_opposite_dir(int dir)
{
    int anti_dir = 4;
    switch(dir) {
    case 0:
        anti_dir = 1;
        break;
    case 1:
        anti_dir = 0;
        break;
    case 2:
        anti_dir = 3;
        break;
    case 3:
        anti_dir = 2;
        break;
    default :
        printf("Invalid use case");
        return 0;
    }
    return anti_dir;
}


