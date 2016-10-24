#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#define PATH_SIZE 500
#define PWD 1
#define CD 2
#define EXPORT 3


void pwd()
{
    char cwd[150];
    getcwd(cwd,sizeof(cwd));

    printf("%s\n",cwd);
}

void cd(char string[])
{
    char cwd[150];
    char *token;

    getcwd(cwd,sizeof(cwd));
    strcat(cwd,"/");

    token=strtok(string," ");
    
    token=strtok(NULL," ");

    if(token==NULL)
    {
        printf("Token is NULL\n");
        char *home=getenv("HOME");
        int status=chdir(home);
        if(status==0)
            printf("Successfully changed \n");
        else
            printf("Some problem\n");

    }
    else
    {
       if(token[0]==' ' || token[0]=='~')
       {
            char *home=getenv("HOME");
            int status=chdir(home);
            if(status==0)
                printf("Successfully changed \n");
            else
                printf("Some problem\n");

       }
       else
       {
           // printf("control is entering here \n");
           int status=chdir(token);
            if(status==0)
                printf("Successfully changed \n");
            else
                printf("Some problem\n");

       }

    }

    getcwd(cwd,sizeof(cwd));
   
    printf("%s\n",cwd);


}

void export_func(char string[])
{
    //check for getenv and set env
    char *token,*value;
    int status;

    token=strtok(string," ");
    token=strtok(NULL," ");
   
    value=getenv(token);
    if(value==NULL)
    {
        printf("Error in getenv\n");
        return;
    }
    else
    {
        ;//working
    }

    status=setenv(token,value,1);
    if(status==0)
    {
        ;//sucessfully exported
    }
    else
    {
        printf("Error in setenv\n");
        return;
    }
   
}


int shell_built_in(char cmd[])
{
    char bkp[PATH_SIZE];
    char *token_cmd;
    int choice;


//    fgets(cmd,PATH_SIZE,stdin);
    cmd[strlen(cmd)-1]='\0';

    //this is necessary since input is taken from fgets
    if(cmd[0]=='\0')
        return 0;

    strcpy(bkp,cmd);
    //we have to check for 3 cmds: cd, pwd, export
    //check if the first word is cd
    token_cmd=strtok(bkp," ");

    if(strcmp(token_cmd,"cd")==0)
        choice=CD;

    else if(strcmp(token_cmd,"pwd")==0)
            choice=PWD;

    else if(strcmp(token_cmd,"export")==0)
             choice=EXPORT;
        

    switch(choice)
    {
        case PWD:    pwd();
                     break;
 
        case CD:     cd(cmd);
                     break;

        case EXPORT: export_func(cmd);
                     break;    
            
    }
    

    return 1;
}
