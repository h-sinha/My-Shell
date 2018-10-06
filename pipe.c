#include "headers.h"

void pipe_exec(char token[200][200], int len)
{
	int idx, pid, prev = 0, num_command = 0;
	int pipe_in, fd[2], length[100], fout, fin;

	pipe_in = 0;
	len++;
	char *command[100][200];
  for (int i = 0; i < 100; ++i)
  {
    for (int j = 0; j < 200; ++j)
    {
      command[i][j] = NULL;
    }

  }

	for (int i = 0; i < len; ++i)
	{
		if(strcmp(token[i], "|") == 0)
		{
			for (int j = prev; j < i; ++j)
			{
				command[num_command][j - prev] = token[j];
			}
			length[num_command] = i - prev; 
			command[num_command][i - prev] = NULL;
			prev = i + 1;
			num_command++;
		}
	}
	for (int i = prev; i < len ; ++i)
	{
		command[num_command][i - prev] = token[i];
	}
	length[num_command] = len - prev; 
	num_command++;


  for (int i = 0; i < num_command - 1; ++i)
	{
		pipe(fd);
		pid = fork();
		if (pid == 0)
    {
    		for (int j = 0; j < length[i]; ++j)
    		{
    			if(strcmp(command[i][j], "<") == 0 )
    			{
              command[i][j] = NULL;
              char *file = command[i][j+1];
              fin = open(file,O_RDONLY);
              if(fin < 0)
              {
                perror("Error");
                return;
              }
              dup2(fin,pipe_in);
              close(fin);
    			}
          else if(strcmp(command[i][j], ">") == 0 || strcmp(command[i][j], ">>") == 0 )
          {

              char *file = command[i][j+1];
              if(strcmp(command[i][j], ">") == 0)
                fout = open(file, O_CREAT|O_WRONLY|O_TRUNC, 0644);
              else if (strcmp(command[i][j], ">>") == 0)
                fout = open(file, O_CREAT | O_APPEND | O_WRONLY, 0644);
              command[i][j] = NULL;
              if(fout < 0)
              {
                perror("Error");
                return;
              }
              dup2(fout,fd[1]);
              close(fout);
          }
    		}
    		if (pipe_in != 0)
        {
          dup2 (pipe_in, 0);
          close (pipe_in);
        }

      	if (fd[1] != 1)
        {
          dup2 (fd[1], 1);
         	close (fd[1]);
      	}
      	execvp (command[i][0], command[i]);
    }
    waitpid(pid,NULL,0);
    pipe_in = fd[0];
    close (fd [1]);
	  pipe_in = fd [0];
  }
  pid  = fork();
	if(pid == 0)
	{
    setpgid(getpid(),10);

    signal(SIGINT, SIG_IGN);
    for (int j = 0; j < length[num_command - 1]; ++j)
    {
          if(strcmp(command[num_command - 1][j], "<") == 0 )
          {
              command[num_command - 1][j] = NULL;
              char *file = command[num_command - 1][j+1];
              fin = open(file,O_RDONLY);
              if(fin < 0)
              {
                perror("Error");
                return;
              }
              dup2(fin,pipe_in);
              close(fin);
          }
          else if(strcmp(command[num_command - 1][j], ">") == 0 || strcmp(command[num_command - 1][j], ">>") == 0 )
          {

              char *file = command[num_command - 1][j+1];
              if(strcmp(command[num_command - 1][j], ">") == 0)
                fout = open(file, O_CREAT|O_WRONLY|O_TRUNC, 0644);
              else if (strcmp(command[num_command - 1][j], ">>") == 0)
                fout = open(file, O_CREAT | O_APPEND | O_WRONLY, 0644);
              command[num_command - 1][j] = NULL;
              if(fout < 0)
              {
                perror("Error");
                return;
              }
              dup2(fout,1);
              close(fout);
          }
    }

	 if (pipe_in != 0)
	 {
  		dup2 (pipe_in, 0);
       close(pipe_in);
  	}
  	execvp (command[num_command - 1][0], command[num_command - 1]);
  	printf("Error: Command not found\n");
  	exit(0);
  }
  waitpid(pid,NULL,0);
  return;
}