#include "headers.h"

char current_path[1000], home_dir[100];
int length_of_home, pid = 1, children = 0, start = 0, cur_max_proc = 0, bgflg = 0, killer = 0 ;

struct child_process{
	char proc_name[100];
	int num;
	pid_t proc_id ;
};
struct child_process childprocess[100];
void zhandler()
{	
	bgflg = 1;
	return;
}
void chandler()
{
	killer = 1;
	return;
}
void fg(int pid)
{
	if (pid == 0) 
	{
        printf("usage: fg <jobNumber>\n");
        return;
    }
	int status;
	signal(SIGINT, chandler);
	signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, zhandler);
    kill(pid,SIGCONT);

   
    while(waitpid(pid,&status,WNOHANG) != pid)
	{
		if(bgflg == 1)break;
		if(killer == 1)break;
	}
	if(bgflg == 1)
	{
		setpgid(pid,pid);
		int f=0;
		for(int i=0;i<100;i++)
		{
			if (childprocess[i].proc_id == pid)
			{
				f = 1;
				break;
			}
		}
		if(!f)
		{
			childprocess[cur_max_proc++].proc_id = pid;
			strcpy(childprocess[cur_max_proc - 1].proc_name, "process");
			childprocess[cur_max_proc - 1].num = cur_max_proc -1;
		}
	}
	else if(killer == 1)
	{
		kill(pid, 9);
	}
    return;
}
void swap(struct child_process *p1, struct child_process *p2)
{
	struct child_process temp;
    temp = *p1;
    *p1 = *p2;
    *p2 = temp;
    return;
}
void sort_proc()
{
	int f=0;
	for (int i = 0; i < 99; ++i)
	{
		for (int j = 0; j < 100 - i - 1; j++)
		{
			if(childprocess[j].num > childprocess[j+1].num)
			{
				swap(&childprocess[j], &childprocess[j + 1]);
			}
		}
	}
	for (int i = 0; i < 100; ++i)
	{
		childprocess[i].num = i + 1;
	}

}
void show_prompt()
{
	struct utsname buf; 
	struct passwd *pw;
	uid_t uid;
	uname(&buf);
	uid = geteuid();
  	pw = getpwuid(uid);
  	char* cur = get_current_path();
  	int len = strlen(cur);
	int flag= 0,i =0;
	for (i = 0; i < length_of_home && i < len; ++i)
	{
		if(home_dir[i] != cur[i])
		{
			flag = 1;
			break;
		}
	}
	if(len < length_of_home)flag = 1;
	int idx = 1;
	if(flag == 0)
	{
		current_path[0] = '~';
		for (i; i < len; ++i)
		{
			current_path[idx++] = cur[i];
		}
	}
	else
	{
		strcpy(current_path, cur);
		idx = len;
	}
	current_path[idx] = '\0';
	printf("\x1b[1;31m" "<%s@%s:%s>""\x1b[1;37m", pw->pw_name, buf.nodename, current_path);
}

void cd(char path[200][200], int len)
{
	int cur;
	if((path[1][0] == '~' && len == 1)  || (len == 0))
	{
		chdir(home_dir);
		current_path[0] = '~';
		current_path[1] = '\0';
		return;
	}
	int err = chdir(path[1]);
	if(err != 0)
	{
		perror(path[1]);
		return;
	}
	return;
}
void exec_fork(char command[200][200], int len)
{
	char *cmd[10];
	int background = 0;
	for (int i = 0; i <= 10; ++i)
	{
		cmd[i] = NULL;
	}
	for (int i = 0; i <= len; ++i)
	{
		if(strcmp(command[i],"&") == 0)
		{
			background = 1;
			break;
		}
		cmd[i] = command[i];
	}
	pid = fork();
	if(pid < 0)
	{
		printf("Fork failed\n");
		return;
	}
	if(pid == 0)
	{
		if(background)
		{
			signal(SIGINT, SIG_DFL);
        	signal(SIGQUIT, SIG_DFL);
        	signal(SIGTTIN, SIG_DFL);
        	signal(SIGTTOU, SIG_DFL);
        	signal(SIGCHLD, SIG_DFL);
			setpgid(getpid(),pid);
		}

		execvp(cmd[0], cmd);
		printf("Error : command not found\n");
		exit(0);
	}
	else
	{

		if(background == 0)
		{
			int status;
			while(waitpid(pid,&status,WUNTRACED) > 0)
			{
				if(bgflg == 1)break;
				if(killer == 1)break;
			}
			if(bgflg == 1)
			{
				childprocess[cur_max_proc++].proc_id = pid;
				setpgid(pid,pid);
				strcpy(childprocess[cur_max_proc - 1].proc_name, cmd[0]);
				childprocess[cur_max_proc - 1].num = cur_max_proc;
			}
			else if(killer == 1)
			{
				kill(pid, 9);
			}
			sort_proc();
			signal(SIGCHLD,SIG_IGN);
		}
		else
		{
			int flag= 0;
			for (int j = 0; j < children; ++j)
			{
				if(childprocess[j].proc_id == 0)
				{
					childprocess[j].proc_id = pid;
					strcpy(childprocess[j].proc_name, cmd[0]);
					childprocess[j].num = ++cur_max_proc;
					flag = 1;
				}
			}
			if(!flag)
			{
				childprocess[children++].proc_id = pid;
				childprocess[children - 1].num = ++cur_max_proc;
				int len = strlen(cmd[0]);
				strcpy(childprocess[children-1].proc_name, cmd[0]);
			}
			signal(SIGCHLD,SIG_IGN);
			printf("Background Process %d\n",pid);
		}
		tcsetpgrp(0, getpid());
	}
}
// len denotes last index
void run_command(char command[200][200], int len)
{
	if (strcmp(command[0], "cd") == 0)
	{
		cd(command, len);
	}
	else if (strcmp(command[0], "fg") == 0)
	{
		if(childprocess[atoi(command[1]) - 1].proc_id == 0)
		{
        	printf("usage: fg <jobNumber>\n");
        	return;
		}
		printf("%s\n",childprocess[atoi(command[1]) - 1].proc_name );
		fg(childprocess[atoi(command[1]) - 1].proc_id);
	}
	else if (strcmp(command[0], "bg") == 0)
	{
		if(childprocess[atoi(command[1]) - 1].proc_id == 0)
		{
        	printf("usage: bg <jobNumber>\n");
        	return;
		}
		printf("%s\n",childprocess[atoi(command[1]) - 1].proc_name );
		bg(childprocess[atoi(command[1]) - 1].proc_id);
	}
	else if (strcmp(command[0], "pwd") == 0)
	{
		pwd();
	}
	else if (strcmp(command[0], "echo") == 0)
	{
		echo(command, len);
	}
	else if(strcmp(command[0], "quit") == 0)
	{
		exit(0);
	}
	else if(strcmp(command[0], "setenv") == 0)
	{
		if(len > 2)
		{
			printf("Wrong input format\n");
		}
		else 
		{
			set_env(command);
		}
	}
	else if(strcmp(command[0], "overkill") == 0)
	{
		int id[100];
		for (int j = 0; j < 100; ++j)
		{
			id[j] = childprocess[j].proc_id;
		}
		overkill(id);
	}
	else if(strcmp(command[0], "unsetenv") == 0)
	{
		if(len < 1)
		{
			printf("Error : No such variable\n");
			return;
		}
		remove_env(command);
	}
	else if(strcmp(command[0], "jobs") == 0)
	{
		int id[100],num[100];
		sort_proc();
		for (int j = 0; j < 100; ++j)
		{
			id[j] = childprocess[j].proc_id;
			num[j] = childprocess[j].num;
		}
		jobs(id,num);
	}
	else if (strcmp(command[0], "ls") == 0)
	{
		int aflag, lflag, length, bg = 0;
		char path[1000];
		for (int i = 0; i < 1000; ++i)
		{
			path[i] = '\0';
		}
		lflag = 0, aflag = 0;
		for (int i = 1; i <= len; ++i)
		{
			if(command[i][0] == '-')
			{
				length = strlen(command[i]);
				for (int j = 1; j < length; ++j)
				{
					if(command[i][j] == 'a')
					{
						aflag = 1;
					}
					else if(command[i][j] == 'l')
					{
						lflag = 2;
					}

				}
			}
			else
			{
				length = strlen(command[i]);
				for (int j = 0; j < length; ++j)
				{
					path[j] = command[i][j];
				}
			}
			if(command[i][0] == '&' && strlen(command[i]) == 1)
			{
				bg = 1;
			}
		}
		ls(path,aflag + lflag, bg);
	}
	else if(strcmp(command[0], "remindme") == 0)
	{
		remindme(command, len);
	}
	else if (strcmp(command[0], "pinfo") == 0)
	{
		if(len == 1)pinfo(command[1]);
		else pinfo(NULL);
	}
	else if(strcmp(command[0], "clock") == 0)
	{
		real_clock(command);
	}
	else if(strcmp(command[0], "kjob") == 0)
	{
		int arg = atoi(command[1]);

		for (int j = 0; j < 100; ++j)
		{
			if(childprocess[j].num == arg)
			{
				kjob(childprocess[j].proc_id, atoi(command[2]));
				return;
			}
		}
	}
	else
	{
		exec_fork(command,len);
	}
	return;
}

// get user command
void get_command(char command[200])
{
	char token[200][200], *ptr;
	for (int i = 0; i < 200; ++i)
	{
			strcpy(token[i],"");
	}
	int len = strlen(command), redirect_flag = 0, pipe_flag = 0;
	if(len == 1 && command[0] == '\n')return;
	if (len > 0 && command[len-1] == '\n')
	{
		command[len - 1] = '\0';
		len--;
	}
	for (int i = 0; i < len; ++i)
	{
		if(command[i] == '<' || command[i] == '>')
		{
			redirect_flag = 1;
		}
		if(command[i] == '|')
		{
			pipe_flag = 1;
		}

	}
	ptr = strtok(command, " \t");
	strcpy(token[0], ptr);
	int i = 0, length = 0;
    while (ptr != NULL) 
    {
        i++;
        ptr = strtok(NULL, " \t");  
		if(ptr != NULL)
		{
			strcpy(token[i], ptr);
			length = i;
		}
    }
    if(pipe_flag == 1)
    	pipe_exec(token, length);
 	else if(redirect_flag == 1)
 		redirection(token, length);
    // length represents number of space separated strings
    else
    run_command(token, length);
}
int get_status(int pid)
{
	return kill(pid, 0);
}
void get_line()
{
	char line[1000],commands[200][200];
	char *ptr;
	fgets (line, 1000, stdin);
	ptr = strtok(line,";");
	strcpy(commands[0], ptr);
	int i = 1;
	while (ptr != NULL) 
    {
        ptr = strtok(NULL, ";");  
		if(ptr != NULL)
		{
			strcpy(commands[i++], ptr);
		}
    }
    for (int j = 0; j < i; ++j)
    {
    	get_command(commands[j]);
    }
}



int main()
{
	char *aux = get_current_path();
	int ret_status;
	length_of_home = strlen(aux);
	for (int i = 0; i < 100; ++i)
	{
		childprocess[i].num = 1000;
		childprocess[i].proc_id = 0;
		strcpy(childprocess[i].proc_name,"aa");
	}
	signal(SIGINT, chandler);
	signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, zhandler);

	pid = getpid();
	setpgid(pid,pid);
	tcsetpgrp(0,pid);

	for (int i = 0; i < length_of_home; ++i)
	{
		home_dir[i] = aux[i];
	}
	while(1)
	{
		if(pid == 0)continue;
		bgflg = 0;
		killer = 0;
		sort_proc();
		show_prompt();
		get_line();
		for (int i=0 ;i<children; ++i)
		{
			if(childprocess[i].proc_id == 0)continue;
			ret_status = get_status(childprocess[i].proc_id);
			if(ret_status == -1)
			{
				printf("%s with PID %d exited normally\n",childprocess[i].proc_name, childprocess[i].proc_id);
				childprocess[i].num = 10000;
				childprocess[i].proc_id = 0;
			}
		}
	}
	return 0;
}