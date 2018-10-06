#ifndef FUNC_H
#define FUNC_H
void real_clock(char command[200][200]);
void clock_signal(int signal);
char *get_current_path();
void echo(char message[200][200], int len);
void get_permissions(char *file);
void ls(char path[100], int flag, int bg);
void pinfo(char *pid);
void pwd();
void remindme(char msg[200][200], int len);
void remove_env(char command[200][200]);
void set_env(char command[200][200]);
void redirection(char commmand[200][200], int len);
void pipe_exec(char commmand[200][200], int len);
void jobs(int id[100], int num[100]);
void kjob(int arg1, int arg2);
void overkill(int id[100]);
void bg(int pid);
// void fg(int pid);
#endif
