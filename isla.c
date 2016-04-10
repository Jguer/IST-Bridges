#include "isla.h"

struct _isla {
    int name;
    pos *coords;
    int bridges_avb;
};

isla *create_isla(int name, int x, int y, int bridges_avb)
{
    isla *new_isla;
    pos *new_pos;

    new_isla = (isla *)malloc(sizeof(isla));
    new_pos = create_pos(x, y);

    new_isla->name = name;
    new_isla->coords = new_pos;
    new_isla->bridges_avb = bridges_avb;

    return new_isla;
}


void print_isla(item got_item)
{
    isla *got_isla = (isla *)got_item;

    fprintf(DEBUG_LOC,
            KBLU "Isla name:" RESET " %d "
            KBLU "Bridges Available:" RESET " %d "
            KBLU "Position:" KYEL " %d, %d\n" RESET,
            got_isla->name, got_isla->bridges_avb, get_x(got_isla->coords), get_y(got_isla->coords));
    return;
}

void free_isla(item got_item)
{
    isla *got_isla = (isla *)got_item;

    /* Free position of island for rich magnates */
    free_pos(got_isla->coords);

    /* Bring democracy to the pointer*/
    free(got_isla);

    return;
}
