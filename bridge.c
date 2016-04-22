#include "bridge.h"

int randomize(int limit)
{
	int generated = 0;
	time_t t;

	srand((unsigned) time(&t));

	return generated = rand()%(limit+1);
}