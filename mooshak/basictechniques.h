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

bool check_for_allzero(list *isla_list);
bool check_for_allconnected(list *isla_list);
bool loner_neighbour(isla *new_isla, stack *got_stack);
bool special_impar_treatment(isla *new_isla, stack *got_stack);
bool n_four(isla *magica, stack *got_stack);
bool n_six(isla *magica, stack *got_stack);
bool n_eight(isla *magica, stack *got_stack);
bool in_side_3(isla *new_isla, stack *got_stack);
bool connect_obvious(stack *got_stack, list *isla_list);

#endif
