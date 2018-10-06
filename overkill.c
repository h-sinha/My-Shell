#include "headers.h"

void overkill(int id[100])
{
	for (int i = 0; i < 100; ++i)
	{
		if(id[i] == 0)continue;
		kill(id[i], 9);
	}
	return;
}