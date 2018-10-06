#include "headers.h"

volatile sig_atomic_t flg = 1;
void clock_signal(int signal)
{ 
  	flg = 0; 
}

void real_clock(char command[200][200])
{
	flg = 1;
	signal(SIGINT, clock_signal); 
	signal(SIGCHLD,SIG_IGN);
    setpgid(getpid(),11);
	int cur = fork();
	char cur_date[5][10], cur_time[5][10], output[5][10];
	if(cur == 0)
	{
    setpgid(getpid(),10);
		signal(SIGINT, SIG_IGN);
		char path[100], line[100],  *p;
		strcpy(path, "/proc/");
		strcat(path, "/driver");
		strcat(path, "/rtc");

		while(flg)
		{
			FILE* fd = fopen(path,"r");
			if(fd < 0)
			{
				perror("Error: ");
				return;
			}
			sleep(atoi(command[2]));
			char *p = NULL;
			while(fgets(line, 100, fd)) 
			{
		        if(strncmp(line, "rtc_time", 8) == 0)
		      	{ 
		      		p = line + 10;
		        	while(*p == ' ' || *p == ':')++p;
		        	p[8] = '\0';
		        	strcpy(output[3], p);
		        }
		        if(strncmp(line, "rtc_date", 8) == 0)
		      	{ 
		      		p = line + 10;
		        	while(*p == ' ' || *p == ':') ++p;
		        	p[10] = '\0';
					char *ptr;
					ptr = strtok(p,"-");
					strcpy(cur_date[0], ptr);
					int i = 1;
					while (ptr != NULL) 
					{
					    ptr = strtok(NULL, "-");  
						if(ptr != NULL)
						{
							strcpy(cur_date[i++], ptr);
						}
					}
					int month = atoi(cur_date[1]);
					char out_month[5];
					if(month == 1)strcpy(out_month,"Jan");
					else if(month == 2)strcpy(out_month, "Feb");
					else if(month == 3)strcpy(out_month, "Mar");
					else if(month == 4)strcpy(out_month, "Apr");
					else if(month == 5)strcpy(out_month, "May");
					else if(month == 6)strcpy(out_month, "June");
					else if(month == 7)strcpy(out_month, "July");
					else if(month == 8)strcpy(out_month, "Aug");
					else if(month == 9)strcpy(out_month, "Sep");
					else if(month == 10)strcpy(out_month, "Oct");
					else if(month == 11)strcpy(out_month, "Nov");
					else if(month == 12)strcpy(out_month, "Dec");

					strcpy(output[0], cur_date[2]);
					strcpy(output[1], out_month);
					strcpy(output[2], cur_date[0]);
		        	break;
		        }

		    }
		    for (int i = 0; i < 4; ++i)
		    {
		    	printf("%s ",output[i]);
		    }
		    printf("\n");
			fclose(fd);
		}
		pid_t pd = getpid();

		char *cmd[10];
		cmd[0] = "kill";
		cmd[1] = "-9";
		sprintf(cmd[3], "%ld", (long) getpid());
		cmd[4] = NULL;
		
	}
	if(cur==0)
		exit(0);
}