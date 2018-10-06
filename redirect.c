#include "headers.h"

void redirection(char token[200][200], int len)
{
	char infile[100], outfile[100], appendfile[100];
	int err = 0;
	char *commmand[100];
	len++;
	for (int i = 0; i < len; ++i)
	{
		commmand[i] = token[i];
	}
	commmand[len] = NULL;
	int pid, inpflag = 0, outflag = 0, appflag = 0;
	int prev = 0;
	pid = fork();
	if (pid==0)
	{	
    setpgid(getpid(),10);

		signal(SIGINT, SIG_IGN);
		for (int i = 0; i < len; ++i)
		{
			if (strcmp(commmand[i], ">>") == 0)
			{
				strcpy(appendfile, commmand[i + 1]);
				commmand[i] = NULL;
				appflag = 1;
			}
			else if (strcmp(commmand[i],"<") == 0) 
			{
				strcpy(infile, commmand[i + 1]);
				commmand[i] = NULL;
				inpflag = 1;
			}
			else if(strcmp(commmand[i], ">") == 0)
			{
				strcpy(outfile, commmand[i + 1]);
				commmand[i] = NULL;
				outflag = 1;
			}

		}
		if(inpflag == 1)
		{
			int fin;
			fin = open(infile,O_RDONLY);
			if (fin<0)
			{
				perror("Could not open input file"); 
				exit(0);
			}
			dup2(fin,0);
			close(fin);
		}	
		if (outflag == 1)
		{
			int fout;	
			fout = open(outfile, O_CREAT|O_WRONLY | O_TRUNC, 0644);
			if (fout<0)
			{
				perror("Could not open output file"); 
				exit(0);
			}
			dup2(fout,1);
			close(fout);
		}
		if (appflag == 1)
		{
			int fapp;
			fapp = open(appendfile, O_CREAT | O_APPEND | O_WRONLY, 0644);
			if (fapp<0)
			{
				perror("Could not open output file"); exit(0);
			}
			dup2(fapp,1);
			close(fapp);
		}		
		if (execvp(commmand[0],commmand)<0)	
		{
			err = 1;
			printf("%s: Command doesn't exist\n", commmand[0]);
		}
		

	}
	else
	{
		int status;
		while(waitpid(pid,&status,0) > 0);
	}
	if(err)
	{
		printf("%s with process id: %d exited normally\n",commmand[0],pid);
	}					
	
}
