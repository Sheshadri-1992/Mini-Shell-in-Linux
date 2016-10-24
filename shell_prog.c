#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <signal.h>
#include "parse_string.h"
#include "multi_pipe.h"
#include "new_pipe.h"
#include "shell_builtin.h"
#define FILE_NAME "./history_file"

/*GLOBAL COMMAND COUNT */
int global_command_count=0;
FILE *fp;

/*Checks for the presence of a pipe */
int check_for_pipe(char string[])
{
    int flag=0,i=0;

    while(string[i]!='\0')
    {
        if(string[i]=='|')
            flag=1;
        i++;
    }

    return flag;

}


/*Checks for the presence of a shell built in */
int check_for_shell_built_in(char string[])
{
     char bkp[100];
     char *token_cmd;

     //copy the string to bkp so that the original wont be modified
     strcpy(bkp,string);
     bkp[strlen(bkp)-1]='\0';
     
     token_cmd=strtok(bkp," ");  
     

     if(strcmp(token_cmd,"cd")==0)
                 return 1;

     else if(strcmp(token_cmd,"pwd")==0)
                 return 1;

     else if(strcmp(token_cmd,"export")==0)
                 return 1;

     return 0;
}


/*checks for the presence of history command*/
int check_for_history(char string[])
{
     char bkp[100];
     char *token_cmd;

     //copy the string to bkp so that the original wont be modified
     strcpy(bkp,string);
     bkp[strlen(bkp)-1]='\0';
     
     token_cmd=strtok(bkp," ");  
     

     if(strcmp(token_cmd,"history")==0)
                 return 1;

    return 0;
}

/*checks for the presence of echo */
int check_for_echo(char string[])
{
     char bkp[100];
     char *token_cmd;

     //copy the string to bkp so that the original wont be modified
     strcpy(bkp,string);
     bkp[strlen(bkp)-1]='\0';
     
     token_cmd=strtok(bkp," ");  
     

     if(strcmp(token_cmd,"echo")==0)
                 return 1;


    return 0;
}


/*checks for the presence of an equality */
int check_for_equal(char string[])
{
    int flag=0,i=0;

    while(string[i]!='\0')
    {
        if(string[i]=='=')
            flag=1;
        i++;
    }

    return flag;
}



/*Do the echo parsing */
void echo_parsing(char string[])
{
    int i=0;
    char bkp[1000];
    strcpy(bkp,string);

    bkp[strlen(bkp)-1]='\0';

//    printf("Control check 2");
    //eliminate beginning spaces if any
    while(bkp[i]==' ')
        i++;
    i=i+5;

    while(bkp[i]!='\0')
    {
        if(bkp[i]!='$')
        {
            printf("%c",bkp[i]);
            i++;
        }
        else
        {
            int j=i+1;
            while(bkp[i]!=' ' && bkp[i]!='\0' && bkp[i]!='"')
                i++;

            char *env_var=(char *)malloc(sizeof(char)*(i-j+2));
            int k=0;
            while(j<i)
            {
                env_var[k]=bkp[j];            
                k++;
                j++;
            }

            env_var[k]='\0';
            char *value=getenv(env_var);
            printf("%s",value);

        }
            
    }
    printf("\n");
}


/*convert integer to string */
int toString(char a[]) 
{
       int c, sign, offset, n;
       
              
       if (sign == -1) 
       {  
           // Set starting position to convert
             offset = 1;
       }
       else 
       {
             offset = 0;
       }
             
       n = 0;
               
       for (c = offset; a[c] != '\0'; c++)
       {
             n = n * 10 + a[c] - '0';

       }
                 
       if (sign == -1) 
       {
             n = -n;
       }
                   
       return n;
}

/*code to handle ctrl-C */
void sigint_handler(int signum)
{
    //this will reset the handler so that it can handle subsequent signals
    signal(SIGINT,sigint_handler);
    fflush(stdout);
}


/*code for !bang */

void execute_bang(char command[])
{

     fclose(fp);
    
     //now do the parsing
     char bkp[1000],number[10];
     strcpy(bkp,command);
     bkp[strlen(bkp)-1]='\0';
     
     int i=1,j=0;
     char *token=NULL;
     token=strtok(bkp,"!");
     int line_count=0;
     int found=0;

     //FILE VARIABLES
      char *line=NULL;
      size_t len=0;
      ssize_t read;
 
/*     while(bkp[i]!=' ' && bkp[i]!='\0')
     {
        number[j]=bkp[i];
         i++;
         j++;
     }*/

     printf("token is %s\n",token);
    int line_number=toString(token);

   printf("The converted number is %d\n",line_number);
    
    fp=fopen(FILE_NAME,"r");
    if(fp==NULL)
    {
        perror("File open error\n");
        fclose(fp);
        return;
    }
    else
    {
        while((read=getline(&line,&len,fp)) != -1)
        {
            line_count++;
            if(line_count==line_number)
            {
                found=1;
                break;
            }
        }
 
    }

    if(found==1)
    {
        // extract and execute the command
        printf("The command is %s\n",line);
        int i=0;
        char bang_command[150];
        while(line[i]!=' ')
        {
            i++;
        }
        
        int j=0;
        while(line[i]!='\0')
        {
            bang_command[j]=line[i];
            i++;
            j++;
        }

        bang_command[j]='\0';
//        bang_command[j+1]='\';

        printf("This is the final bang command %s",bang_command);

        //we need to pass this to shell built in and stuff
        int shell_builtin=0;
        int echo_without_pipe=0;
        int echo_check=0;
        int history=0;
        int check_equal=0;
        int pipe_flag=0;

         //check for pipe
       pipe_flag=check_for_pipe(bang_command);



        //check for shell built-ins like cd pwd and export
        shell_builtin=check_for_shell_built_in(bang_command);
        if(shell_builtin==1)
        {        
            //do the shell builtin stuff 
            shell_built_in(bang_command);
            return;
        }

        //check for the keyword history
        history=check_for_history(bang_command);
        if(history==1)
        {
            //display the history and continue
            //here the file is already open, we need to close it, open it read mode and then close and open it in append again
    
            //we need to check if history has an argument following it
            char bkp[100];
            char *token=NULL;
            strcpy(bkp,bang_command);
            bkp[strlen(bkp)-1]='\0';
            token=strtok(bkp," ");
            token=strtok(NULL," ");     

            if(token==NULL)
            {                        
                fclose(fp);
                
                fp=fopen(FILE_NAME,"r");
                if(fp==NULL)
                {
                    perror("Nothing in file\n");
                    return;
                }   
                else
                {
                    while((read=getline(&line,&len,fp)) != -1)
                    {
                        printf("%s",line);
                    }
                }
                fclose(fp);


                //now just open it in append mode again
                fopen(FILE_NAME,"a+");

            }
            else
            {
                //we have to display only that many records from the history
                int no_of_records=toString(token);
                printf("We need to display %d number of records\n",no_of_records);
                int total_records=0;


                /*finds out total number of records */
                fclose(fp);
                
                fp=fopen(FILE_NAME,"r");
                if(fp==NULL)
                {
                    perror("Nothing in file\n");
                }   
                else
                {
                    while((read=getline(&line,&len,fp)) != -1)
                    {
                        total_records++;
                    }
                }
                fclose(fp);
                //now we have number of records in the file
    
                int records_to_skip=total_records-no_of_records;
                int counter=0;

                fp=fopen(FILE_NAME,"r");
                if(fp==NULL)
                {
                    perror("Nothing in file\n");
                }   
                else
                {
                    while((read=getline(&line,&len,fp)) != -1)
                    {
                        if(counter>records_to_skip-1)
                            printf("%s",line);

                        counter++;
                    }
                }
                fclose(fp);

                fp=fopen(FILE_NAME,"a+");

            }

            return;
            
        }

        //check for equal keyword
        check_equal=check_for_equal(bang_command);
        if(check_equal==1)
        {
            char *before_equal;
            char *after_equal;
            int status;

            before_equal=strtok(bang_command,"=");
            after_equal=strtok(NULL,"=");

            printf("value before equal is %s, valu after equal is %s \n",before_equal,after_equal);

            status=setenv(before_equal,after_equal,1);
            if(status==0)
            {
                printf("Value succesfully set!!\n");
            }
            else
            {
                perror("Something wrong in setting value!\n");
            }
            return;
        }

        echo_check=check_for_echo(bang_command);
        if(echo_check==1 && pipe_flag==0)
        {
            //do the echo logic
            //this logic does only this
            printf("Control check 1\n");
            echo_parsing(bang_command);

            return;
        }



        if(strcmp(bang_command,"exit\n")==0)
            exit(0);




    }
    else
    {
        perror("Required history not found\n");
    }

    fclose(fp);





     fp=fopen(FILE_NAME,"a+");
}

extern int pipe_counter;
extern char **all_my_args[1000];
int main()
{
    char command[100];
    int pipe_flag=0;
    int shell_builtin=0;
    int echo_without_pipe=0;
    int echo_check=0;
    int history=0;
    int check_equal=0;
    int my_temporary_stdout_fd=0;
    int saved_stdout;
    int check_bang=0;

    /*FILE VARIABLES*/
    char *line=NULL;
    size_t len=0;
    ssize_t read;

    //create a file if not for saving history
    fp=fopen(FILE_NAME,"a+");
    fclose(fp);    
    //file creation part ended

    //open the file to count the number of lines
    fp=fopen(FILE_NAME,"r");
    int line_counter=0;
    if(fp==NULL)
    {
        line_counter=0;
    }
    else
    {
        while((read=getline(&line,&len,fp)) != -1)
        {
            line_counter++;
        }
    }
    fclose(fp);
    //counted the number of lines    

    signal(SIGINT,sigint_handler);
//    signal(SIGINT,sigint_handler_2);
    fp=fopen(FILE_NAME,"a+");

    while(1)
    {
        check_bang=0;
        saved_stdout = dup(0);
        dup2(my_temporary_stdout_fd, 0);
        fflush(stdout);
        printf("swami@swami-Dell-System-Inspiron-N4110:");
        fgets(command,100,stdin);
//        printf("command is %s",command);
        if(strcmp(command,"\n")==0)
            continue;

        if(command[0]=='!')
            check_bang=1;

        if(check_bang==1)
        {
            //do the processing
            execute_bang(command);
            continue;
        }

        //write into the file, the command and increment the count
        fprintf(fp,"%d %s",line_counter,command);        
        line_counter++;



        //check for pipe
        pipe_flag=check_for_pipe(command);



        //check for shell built-ins like cd pwd and export
        shell_builtin=check_for_shell_built_in(command);
        if(shell_builtin==1)
        {        
            //do the shell builtin stuff 
            shell_built_in(command);
            continue;
        }

        //check for the keyword history
        history=check_for_history(command);
        if(history==1)
        {
            //display the history and continue
            //here the file is already open, we need to close it, open it read mode and then close and open it in append again
    
            //we need to check if history has an argument following it
            char bkp[100];
            char *token=NULL;
            strcpy(bkp,command);
            bkp[strlen(bkp)-1]='\0';
            token=strtok(bkp," ");
            token=strtok(NULL," ");     

            if(token==NULL)
            {                        
                fclose(fp);
                
                fp=fopen(FILE_NAME,"r");
                if(fp==NULL)
                {
                    perror("Nothing in file\n");
                    continue;
                }   
                else
                {
                    while((read=getline(&line,&len,fp)) != -1)
                    {
                        printf("%s",line);
                    }
                }
                fclose(fp);


                //now just open it in append mode again
                fopen(FILE_NAME,"a+");

            }
            else
            {
                //we have to display only that many records from the history
                int no_of_records=toString(token);
  //              printf("We need to display %d number of records\n",no_of_records);
                int total_records=0;


                /*finds out total number of records */
                fclose(fp);
                
                fp=fopen(FILE_NAME,"r");
                if(fp==NULL)
                {
                    perror("Nothing in file\n");
                }   
                else
                {
                    while((read=getline(&line,&len,fp)) != -1)
                    {
                        total_records++;
                    }
                }
                fclose(fp);
                //now we have number of records in the file
    
                int records_to_skip=total_records-no_of_records;
                int counter=0;

                fp=fopen(FILE_NAME,"r");
                if(fp==NULL)
                {
                    perror("Nothing in file\n");
                }   
                else
                {
                    while((read=getline(&line,&len,fp)) != -1)
                    {
                        if(counter>records_to_skip-1)
                            printf("%s",line);

                        counter++;
                    }
                }
                fclose(fp);

                fp=fopen(FILE_NAME,"a+");

            }

            continue;
            
        }

        //check for equal keyword
        check_equal=check_for_equal(command);
        if(check_equal==1)
        {
            char *before_equal;
            char *after_equal;
            int status;

            before_equal=strtok(command,"=");
            after_equal=strtok(NULL,"=");

    //        printf("value before equal is %s, valu after equal is %s \n",before_equal,after_equal);

            status=setenv(before_equal,after_equal,1);
            if(status==0)
            {
                printf("Value succesfully set!!\n");
            }
            else
            {
                perror("Something wrong in setting value!\n");
            }
            continue;
        }

        echo_check=check_for_echo(command);
        if(echo_check==1 && pipe_flag==0)
        {
            //do the echo logic
            //this logic does only this
        //    printf("Control check 1\n");
            echo_parsing(command);

            continue;
        }



        if(strcmp(command,"exit\n")==0)
            exit(0);


        
        //echo and no pipe handle echo and pipe handle it seperately

    //    if(pipe_flag==1)
      //  {
            char ***all_my_args1 = parse_function(command);
      //      printf("This is a global extern variable %d\n",pipe_counter);
        
  //          int saved_stdout=dup(0);
            //things are successfully copied! :)
            // myfunc(all_my_args,pipe_counter); //pipe_counter+1 dont forget it :)

        //----------------------------------MULTI LEVEL PIPE---------------------------------//
            loop_pipe(all_my_args);
            //pipe has to be reset because next time it has to be ready for one more multi pipe
             pipe_counter=0;
         //    continue;               
//            dup2(saved_stdout, 0);
//            close(saved_stdout);
       // }

        dup2(saved_stdout, 0);
        close(saved_stdout);

     }

    //file opened before the while loop
    close(fp);
    return 0;
}
