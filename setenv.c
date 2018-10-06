#include "headers.h"

void set_env(char command[200][200])
{
	int ret;
	if(command[2] != NULL)
	{
		ret = setenv(command[1], command[2], 0);
		if(ret < 0)
			perror("Error ");
	}
	else 
	{
		ret = setenv(command[1], " ", 0);
		if(ret < 0)
			perror("Error  ");
	}
	return;
}

void remove_env(char command[200][200])
{
	int ret;
	ret = unsetenv(command[1]);
	if(ret < 0)
		perror("Error : ");
	return;
}
