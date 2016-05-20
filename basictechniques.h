/*********************************************************************************
*     File Name           :     basictechniques.h
*     Created By          :     jguer
*     Creation Date       :     [2016-05-17 01:40]
*     Last Modified       :     [2016-05-17 02:01]
*     Description         :     Obvious storage
**********************************************************************************/
#ifndef BBHeader
#define BBHeader

#include "bridge.h"

bool check_for_allconnected(list *isla_list);

bool is_connectable(isla *isla_a, isla *isla_b, int adj_index, stack *got_stack, map *got_map);
bool connect_obvious(stack *got_stack, list *isla_list, map *got_map);

#endif
