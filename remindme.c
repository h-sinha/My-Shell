#include "headers.h"

void remindme(char msg[200][200], int len)
{
	signal(SIGCHLD,SIG_IGN);
    setpgid(getpid(),11);
	int id = fork();
	if(id == 0)
	{
    setpgid(getpid(),10);

		signal(SIGINT, SIG_IGN);
		sleep(atoi(msg[1]));
		printf("\n");
		printf("Reminder: ");
		for (int j = 2; j <= len; ++j)
		{
			printf("%s ",msg[j]);
		}
		exit(0);
	}
	return;
}