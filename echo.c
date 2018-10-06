#include "headers.h"

void echo(char message[200][200], int len)
{
	for (int i = 1; i <= len; ++i)
	{
		printf("%s ",message[i]);
	}
	printf("\n");
}
