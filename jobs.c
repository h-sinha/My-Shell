#include "headers.h"

void jobs(int id[100], int num[100])
{
	char path[100], exepath[100], line[100], *p;
	strcpy(path, "/proc/");
	int cur = 1;
    DIR * dp = opendir(path);
	struct dirent * ptr;
    ptr = readdir(dp);
    for (int i = 0; i < 100; ++i)
    {
    	struct stat st;
        char aux[200], state[200], name[200];
        strcpy(aux, path);
		sprintf(aux + strlen(aux), "%ld", (long) id[i]);
    	stat(aux, &st);
        strcat(aux, "/");
        strcat(aux, "status");
        FILE* fd = fopen(aux,"r");
		if(fd <= 0 || id[i]==0)
		{
			continue;
		}
		printf("[%d]\t",num[i]);
		while(fgets(line, 100, fd)) 
		{
	        if(strncmp(line, "Name:", 5) != 0)
	            continue;
	        p = line + 6;
	        while(*p == ' ') ++p;
	        strcpy(name, p);
	        name[strlen(name) - 1] = '\0';
	        break;
	    }
	   	while(fgets(line, 100, fd)) 
		{
	        if(strncmp(line, "State:", 6) != 0)
	            continue;
	        p = line + 7;
	        while(*p == ' ') ++p;
			strcpy(state, p);
	        state[strlen(state) - 1] = '\0';
	        break;
	    }
	    printf("%s\t%s [%d]\n",state,name,id[i] );
	    fclose(fd);
    }
    closedir(dp);

}