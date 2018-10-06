#include "headers.h"

void fg(int pid)
{
	if (pid == 0) 
	{
        printf("usage: fg <jobNumber>\n");
        return;
    }
    tcsetpgrp(0, pid);
	int status;
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, zhandler);
    waitpid(pid, &status, WUNTRACED);
    signal(SIGTTOU, SIG_IGN);
    tcsetpgrp(0, getpid());
    signal(SIGTTOU, SIG_DFL);
    return;
}
