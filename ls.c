#include "headers.h"

int dir = 0;
void get_permissions(char *file)
{
	int err;
	struct stat statRes;
    char aux[100];
	err = stat(file, &statRes);
	mode_t bits = statRes.st_mode;
	if(bits && S_ISREG(bits))  printf("-");
    else if(bits && S_ISDIR(bits)){ printf("d");dir = 1;}
    else if(bits && S_ISFIFO(bits))  printf("|");
    else if(bits && S_ISSOCK(bits))  printf("s");
    else if(bits && S_ISCHR(bits))  printf("c");
    else if(bits && S_ISBLK(bits))  printf("b");
    else  printf("l");


	if((bits & S_IRUSR) == 0)printf("-");
	else printf("r");

	if((bits & S_IWUSR) == 0)printf("-");
	else printf("w");

	if((bits & S_IXUSR) == 0)printf("-");
	else printf("x");

	if((bits & S_IRGRP) == 0)printf("-");
	else printf("r");

	if((bits & S_IWGRP) == 0)printf("-");
	else printf("w");

	if((bits & S_IXGRP) == 0)printf("-");
	else printf("x");

	if((bits & S_IROTH) == 0)printf("-");
	else printf("r");

	if((bits & S_IWOTH) == 0)printf("-");
	else printf("w");

	if((bits & S_IXOTH) == 0)printf("-");
	else printf("x");

	return;
}
// ls command
void ls(char path[100], int flag, int bg) 
{
	if (path[0] == '\0' || path[0] == '&')
	{
		path = get_current_path();
	}
    DIR * dp = opendir(path);
    if(dp == 0) 
    {
        perror("Error");
        return;
    }
    struct dirent * ptr;
    ptr = readdir(dp);
    while(ptr != NULL)
    {
    	struct stat st;
        char aux[200];
        strcpy(aux, path);
        strcat(aux, "/");
        strcat(aux, ptr->d_name);
    	stat(aux, &st);
    	if(ptr->d_name[0] == '.' && !(flag&1))
    	{
    		ptr = readdir(dp);
    		continue;
    	}
    	if(flag > 1)
    	{
            dir = 0;
    		get_permissions(aux);
    		printf("\t");
	    	printf("%ld\t",st.st_nlink );
	    	printf("%s\t",getpwuid(st.st_uid)->pw_name );
	    	printf("%s\t",getpwuid(st.st_gid)->pw_name );
	    	printf("%ld\t",st.st_size );
	    	char * aux =  ctime(&st.st_mtime);
	    	int len = strlen(aux);
	    	aux[len - 1] = '\t';
	    	printf("%s",aux );
    	}
        if(dir)
            printf("\x1b[1;34m""%s\t""\x1b[1;37m", ptr->d_name);
        else
            printf("%s\t", ptr->d_name);
        if (flag > 1)
       	{
        	printf("\n");
        }
    	ptr = readdir(dp);
    }
    printf("\n");
    closedir(dp);
    return;
}