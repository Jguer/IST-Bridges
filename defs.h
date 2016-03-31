/*********************************************************************************
*     File Name           :     Projects/bridges-aed/defs.h
*     Created By          :     jguer
*     Creation Date       :     [2016-03-31 15:12]
*     Last Modified       :     [2016-03-31 15:22]
*     Description         :
**********************************************************************************/
#ifndef defsHeader

#define defsHeader
#include <stdio.h>
#include <stdlib.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"

typedef void *item;
typedef enum
{
    FALSE=0,
    TRUE=1
} bool;

typedef struct _pos pos;

void memory_error(char *msg);
/* POSITION */
pos *create_pos(int x,int y);

int get_x(pos *gotpos);
int get_y(pos *gotpos);

void free_pos(item pos_to_free);
void print_position(item gotpos);

#endif
