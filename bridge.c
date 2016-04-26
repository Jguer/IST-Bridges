#include "bridge.h"

int randomize(int limit)
{
	int generated = 0;
	time_t t;

	srand((unsigned) time(&t));

	return generated = rand()%(limit+1);
}

/*find the next isla on a row*/
isla* find_next_isla_x(map *got_map, int x, int static_y, int x_max)
{
	isla *new;
	while(new == NULL && x < x_max)
	{
		new = get_tile(got_map, x, static_y);
		x++;
	}
	return new;
}

/*find the next isla on a column*/
isla* find_next_isla_y(map *got_map, int static_x, int y, int y_max)
{
	isla *new;
	while(new == NULL && y < y_max)
	{
		new = get_tile(got_map, static_x, y);
		y++;
	}
	return new;
}

/*sets all adjacent islas in a column*/
void find_adj_y(isla* active_row_node, map *got_map)
{
	isla *new, *new_next;
	int y_max, static_x, y;

	new = active_row_node; 											/* new gets the first isla on the column */
	y_max = get_y_max(got_map); 								
	static_x = get_x(get_pos_isla(active_row_node));				/* gets which column we are working on */

	while(new != NULL)
	{
		y = get_y(get_pos_isla(new));								/* gets the row from which we are working on */
		new_next = find_next_isla_y(got_map, static_x, y, y_max);	/* gets the next isla in that column */
		if(new_next != NULL)										/* if an isla is actually found, it's an adjacent */
		{
			set_adj_isla(new, new_next, 1);
			set_adj_isla(new_next, new, 0);
		}
		new = new_next;												/* start in the next found isla */
	}
}

void find_adj(map* got_map)
{
	isla *new, *new_next;
	int y, y_max, x;
	y_max = get_y_max(got_map);

	new = find_next_isla_x(got_map, 1, 1, y_max);					/* find the isla closest to map origin */

	while(y < y_max)												/* check till last row */
	{
		while(new != NULL)											/* check till last cloumn */
		{
			y = get_y(get_pos_isla(new));
			x = get_x(get_pos_isla(new));
			new_next = find_next_isla_x(got_map, x+1, y, y_max);
			if(new_next != NULL)									/* if the row has more islas */
			{
				set_adj_isla(new, new_next, 2);
				set_adj_isla(new_next, new, 3);
			}

			find_adj_y(new, got_map);								/* find islas in  all column now */
			new = new_next;
		}
		y++;
	}
}

