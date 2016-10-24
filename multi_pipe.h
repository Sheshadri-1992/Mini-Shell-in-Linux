#ifndef MULTI_PIPE_H
#define MULTI_PIPE_H


/*struct command
 {
     const char **argv;
 };*/
void sigint_handler_2(int signum);

int spawn_proc(int in,int out,char **cmd);
int fork_pipes(int n,char ***cmd);
int somefunc();
void myfunc(char ***array,int length);

#endif
