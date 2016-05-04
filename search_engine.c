#include "search_engine.h"

bool is_connectable(isla *isla_eins, isla *isla_zwei)
{
    return TRUE;
}

void engine_static_fire()
{

}

void DFS_engine(list *head, isla *edgy, bool *visited)
{
    list *auximoron;
    isla *_adj = NULL;
    unsigned int i = 0;
    visited[get_name_isla(edgy)] = TRUE;

    printf("Digging Isla %d \n", get_name_isla(edgy));

    /* Travel all nodes, list implementation may be underkill*/
    /* for(auximoron = head; auximoron != NULL; auximoron ); */
    for(_adj = get_adj_isla(edgy, i) ; i < 4; i++)
    {
        printf("Checking for Isla %d, I: %d \n", get_name_isla(edgy),i);
        /* Check if exists and check if visited*/
        if(_adj != NULL )
        {
            printf("Howdy Neighbour \n Flanders %d - %p \n", get_name_isla(_adj), _adj);
            /* If islas are good for connect*/
            if(is_connectable(edgy, _adj) == TRUE)
            {
            }
            if( visited[get_name_isla(_adj)] != TRUE)
            {
                DFS_engine( head, _adj, visited);
            }
        }

    }

    return;
}

void engine_dry_run()
{

}


