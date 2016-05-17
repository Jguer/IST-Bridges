/*********************************************************************************
*     File Name           :     Projects/bridges-aed/defs.h
*     Created By          :     jguer
*     Creation Date       :     [2016-03-31 15:12]
*     Last Modified       :     [2016-05-17 01:44]
*     Description         :
**********************************************************************************/
#ifndef defsHeader
#define defsHeader
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"

#define DEBUG_LOC stdout

#define NO_SOL 10
#define GOT_SOL 11
#define ALL_CONCT 12
#define NOT_ALL_CONCT 13

typedef void *item;
typedef enum
{
    FALSE=0,
    TRUE=1
} bool;

typedef struct _pos pos;

char *get_filename_ext(char *filename);
void file_error(char *msg);
void memory_error(char *msg);

void assign_values(int* vector, int* n_args, ...);

/* POSITION */
pos *create_pos(int x,int y);

int get_x(pos *gotpos);
int get_y(pos *gotpos);

void free_pos(item pos_to_free);
void print_pos(item gotpos);

int get_opposite_dir(int dir);

#endif

