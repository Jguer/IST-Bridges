/*********************************************************************************
*     File Name           :     basictechniques.h
*     Created By          :     jguer
*     Creation Date       :     [2016-05-17 01:40]
*     Last Modified       :     [2016-05-17 02:01]
*     Description         :     Obvious storage
**********************************************************************************/
#ifndef BBHeader
#define BBHeader

#include "search_engine.h"

bool loner_neighbour(isla *new_isla, stack *got_stack);
bool special_impar_treatment(isla *new_isla, stack *got_stack);
bool n_four(isla *magica, stack *got_stack);
bool n_six(isla *magica, stack *got_stack);
bool n_eight(isla *magica, stack *got_stack);
bool in_side_3(isla *new_isla, stack *got_stack);

#endif
