#include "headers.h"

void bg(int pid)
{
	if (pid == 0) 
	{
        printf("usage: bg <jobNumber>\n");
        return;
    }
    kill(pid, SIGCONT);
    return;
}
