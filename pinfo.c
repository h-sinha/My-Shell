#include "headers.h"

void pinfo(char *pid)
{
	char path[100], exepath[100], line[100], *p;
	strcpy(path, "/proc/");
	strcpy(exepath, "/proc/");
	if(!pid)
	{
		sprintf(path + strlen(path), "%ld", (long) getpid());
		sprintf(exepath + strlen(exepath), "%ld", (long) getpid());
		printf("pid - %d\n",getpid() );
	}
	else
	{ 
		strcat(path, pid);
		strcat(exepath, pid);
		printf("pid - %s\n",pid);
	}
	strcat(path, "/status");
	strcat(exepath, "/exe");
	FILE* fd = fopen(path,"r");
	if(fd <= 0)
	{
		printf("No such process\n");
		return;
	}
	while(fgets(line, 100, fd)) 
	{
        if(strncmp(line, "State:", 6) != 0)
            continue;
        p = line + 7;
        while(*p == ' ') ++p;

        printf("Process Status - %s", p);
        break;
    }
    while(fgets(line, 100, fd)) 
	{
        if(strncmp(line, "VmSize:", 6) != 0)
            continue;
        p = line + 7;
        while(*p == ' ') ++p;

        printf("Memory - %s", p);
        break;
    }
    char *out = realpath(exepath,NULL);
    if(out == NULL)
    {
    	printf("Permission denied\n");
    	return;
    }
    printf("Executable path - %s\n",out);
    fclose(fd);
}