#include<stdio.h>
#include<unistd.h>
#include "multi_pipe.h"
#include <stdlib.h>
#include <signal.h>
//trying to implement multiple piple

//                 ---------------------------------------------------------
// FD0(read end)  |                                                         | FD1(write end)
//                 ---------------------------------------------------------

/*struct command
{
    const char **argv;
};*/

//int index=0;

 void sigint_handler_2(int signum)
 {
     //this will reset the handler so that it can handle subsequent signals
         signal(SIGINT,sigint_handler_2);
         exit(0);
         fflush(stdout);
 }


int spawn_proc(int in,int out,char **cmd)
{
    pid_t pid;
//    signal(SIGINT,sigint_handler);

    if((pid = fork())==0)
    {
        signal(SIGINT,sigint_handler_2);
        if( in!=0)
        {
            dup2(in,0);
            close(in);
        }

        if( out!=1)
        {
            dup2(out,1);
            close(out);
        }

        printf("This is entereing here\n");
       return  execvp(cmd[0],(char * const *)cmd);
    }
    else
    {
        wait(0);
    }
    
    return pid;
}


int fork_pipes(int n, char ***cmd)
{
    int i;
    int in,fd[2];
    
    printf("if at all n is passed what is it %d \n",n);
    in=0;

    for(i=0;i<n-1;i++)
    {
        pipe(fd);
        int x;
        // scanf("%d",&x);
        // printf("i is %d",i);
        // printf("Cmd[%d][0] is %s",i,cmd[i][0]);
        // scanf("%d",&x);
        spawn_proc(in,fd[1],cmd[i]);
        close(fd[1]);
        in = fd[0];

    }

    // int x;
    // scanf("%d",&x);

    if(in!=0)
        dup2(in,0);

   printf("value of i is %d\n",i);
   printf("Whats happening here guys! cmd[%d][0]= %s \n",i,cmd[i][0]);
   
   
   // pid=fork();

   // if(pid==0)
   int j=0;
   while(cmd[j]!=NULL)
   {
    printf("%s\n",cmd[j][0]);
    j++;


   }
   
   execvp(cmd[i][0],(char * const *)cmd[i]);
   
   // else
   // {
   //      wait(0);
   // }

}


int somefunc()
{
        
/*    const char *ls[]={"ls", "-l", 0};
    const char *awk[]={"awk", "{print $1}", 0};
    const char *sort[]={"sort",0};
    const char *uniq[]={"uniq",0};

    struct command cmd[]={ {ls}, {awk}, {sort}, {uniq} };
    fork_pipes(4,cmd);*/
    return 0;
}

void myfunc(char ***array,int length)
{
    printf("is the control coming here at all? the length is %d\n",length);

    


    //lets see if the array is coming there properly
    printf("First argument %s",array[0][0]);
    // printf("Second argument %s",array[1][0]);

    // int make;
    // scanf("%d",&make);
    
    fork_pipes(length,array);


    // free(array);

}



