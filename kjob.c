#include "headers.h"

void kjob(int arg1, int arg2)
{
	if(arg1 ==0 || arg2 == 0)
	{
		printf("Incorrect syntax\n");
		return;
	}
	kill(arg1, arg2);
	return;
}
