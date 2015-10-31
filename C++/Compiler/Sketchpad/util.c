#include <stdio.h>
#include "util.h"

void error(int line_number)
{
	fprintf(stderr, "draw2: %d, error.\n", line_number);
}