#include "isla.h"

struct _isla {
    int name;
    int bridges_avb;
};

isla *create_isla(int name, int x, int y, int bridges_avb)
{
    isla *new_isla;

    new_isla = (isla *)malloc(sizeof(isla));
    if(new_isla == NULL)
        memory_error("Unable to create structure isla");

    new_isla->name = name;
    new_isla->bridges_avb = bridges_avb;

    return new_isla;
}


void print_isla(item got_item)
{
    isla *got_isla = (isla *)got_item;

    fprintf(DEBUG_LOC,
            KBLU "Isla name:" RESET " %d "
            KBLU "Bridges Available:" RESET " %d ",
            got_isla->name, got_isla->bridges_avb);
    return;
}

void free_isla(item got_item)
{
    isla *got_isla = (isla *)got_item;

    /* Bring democracy to the pointer */
    free(got_isla);

    return;
}
