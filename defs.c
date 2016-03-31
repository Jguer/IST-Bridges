/*********************************************************************************
*     File Name           :     Projects/bridges-aed/defs.c
*     Created By          :     jguer
*     Creation Date       :     [2016-03-31 15:21]
*     Last Modified       :     [2016-03-31 15:22]
*     Description         :      Useful definitions for project
**********************************************************************************/

#include "defs.h"

struct _pos {
  int x;
  int y;
};

void memory_error(char *msg)
{
  fprintf(stderr, KRED"Error during memory reserve attempt.\n"KNRM);
  fprintf(stderr, KRED"Msg: %s\n",msg);
  fprintf(stderr, KRED"Exit Program due to unmanaged error.\n"KNRM);

  exit(1);
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

void print_position(item gotpos)
{
  printf("Position: %d %d\n", get_x((pos *) gotpos), get_y((pos *) gotpos));
  return;
}
