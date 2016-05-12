#include "file.h"

/* Reads the integers from one line of the maps file
 * and stores them in vector lineData[] 
 */
int read_line(FILE* map_file, int *lineData)
{
    int scanfresult;
    int i = 0;

    while(i<4)
    {
        scanfresult = fscanf(map_file, "%d", &lineData[i]);

        /* if end of file: 0 */
        if(scanfresult == EOF)
            return 0;
        /* if end of map: -1 */
        else if(lineData[i] == -1)
            return -1;
        i++;
    }
    return 1;
}

/*  Sets up an island by creating its structure
 *  Points the correspondent position on the map to that structure
 *  Add that isla to the list of islas
 */
void setup_isla(isla* isla_item, map* map_item, int *lineData, list *list_item)
{
    isla_item = create_isla(lineData[0], lineData[1], lineData[2], lineData[3]);
    set_tile(map_item, isla_item);
    push_item_to_list(list_item, isla_item);
    return;
}

FILE *change_file_ext(char* original_name)
{
    FILE *outfile;
    int Len;

    Len = strlen(original_name);
    
    while(Len) 
    {
        if(original_name[Len] == '.') 
        {
            original_name[Len] = '\0';
            break;
        }
        Len --;
    }

    strcat(original_name, ".sol");
    outfile = fopen(original_name, "w");

    return outfile;
}

void print_bridge_to_file(isla *new_isla, FILE *outfile)
{
    bridge *got_bridge = NULL, *printer_bridge = NULL;
    int n_bridges = 0, i = 0, dir = 0;

    for(dir=0; dir<4; dir++)
    {
        printer_bridge = NULL;
        n_bridges = 0;

        for(i=0; i<2; i++)
        {
            got_bridge = get_isla_used_bridge(new_isla, dir, i);
            if(got_bridge != NULL && get_bridge_written(got_bridge) == FALSE)
            {
                n_bridges++;
                set_bridge_written(got_bridge, 1);
                printer_bridge = got_bridge;
            }
            
        }

        if(printer_bridge != NULL)
        {
            fprintf(outfile, "%d %d %d\n", 
                get_isla_name(get_points(printer_bridge, 0)),
                get_isla_name(get_points(printer_bridge, 1)),
                n_bridges);
        }

    }
}


void print_created_map(list* isla_list, FILE *outfile)
{
    node *new_node = NULL;
    isla *new_isla = NULL;

    new_node = get_head(isla_list);

    while(new_node != NULL)
    {
        new_isla = get_node_item(new_node);

        print_bridge_to_file(new_isla, outfile);

        new_node = get_next_node(new_node);
    }

    return;
}

void print_output_per_map(map *got_map, FILE *outfile, int mode_result, list* isla_list)
{
    int sol_info = 0;
    int mode = get_map_mode(got_map);

    /* for testing */
    mode_result = ALL_CONCT;

    if(mode == 1)
    {
        switch(mode_result)
        {
            case NO_SOL: sol_info = 0;
            case GOT_SOL: sol_info = 0;
        }
    }
    else if(mode == 2)
    {
        switch(mode_result)
        {
            case ALL_CONCT: sol_info = 2;
            case NOT_ALL_CONCT: sol_info = 1;
            case NO_SOL: sol_info = 1;
        }
    }
    else
    {
        switch(mode_result)
        {
            case ALL_CONCT: sol_info = 2;
            case NO_SOL: sol_info = 1;
        }
    }

    /* print first line */
    fprintf(outfile, "%d %d %d %d\n", 
        get_y_max(got_map), get_x_max(got_map), get_n_islas(got_map), sol_info);

    if(mode_result != NO_SOL)
    {
        print_created_map(isla_list, outfile);
    }

    fprintf(outfile, "-1\n");
    return;
}



