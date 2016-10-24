#include <stdio.h>
#include <string.h>
#define SIZE 65032
#include <stdlib.h>
#include "parse_string.h"

void parseString(char string[]);
char **all_my_args[1000];
int index_3d=0;
int pipe_counter=0;


int return_length();

char*** parse_function(char command[])
{

    //ls ­l | grep "out" | wc | wc | grep "1" | wc , strings can be like this
    //or like this ls ­R my_folder1/ | grep "abc" >out
    char **arguments;
    char command_bak[SIZE];
    int i=0;

    index_3d=0;
    //input the command
 //   fgets(command,SIZE,stdin);
    command[strlen(command)-1]='\0';
    int m=0;
    while(command[m]!='\0')
    {
        if(command[m]=='\n')
        {
            // printf("FOUnd it\n");
            ;
        }
        m++;
    }

    strcpy(command_bak,command);

    // printf("The entered string is %s",command);

    //parsing the string
    while(command[i]!='\0')
    {
        if(command[i]=='|')
            pipe_counter++;

        i++;
    }

    // printf("The number of pipes are %d\n",pipe_counter);

    //we are gonna need pipe +1 number of 2d strings
    arguments=(char **)malloc(sizeof(char *)*(pipe_counter+2));

    //Time to tokenize the string based on '|'
    char *token,delim[2]="|";
    token=strtok(command,delim);

    //copy this into the two dimensional string 
    i=0;
    while(token!=NULL)
    {
        arguments[i]=(char *)malloc(sizeof(char)*strlen(token));
        strcpy(arguments[i],token);
        token=strtok(NULL,delim);
        i++;
    }

    for(i=0;i<(pipe_counter+1);i++)
    {
        // printf("Arguments copied are %s\n",arguments[i]);
        ;
    }

    //Now try to delimit by space, we have a problem here what if we have statements liek echo "whats up doc?!? " | wc ??

    // all_my_args=(char ***)malloc(sizeof(char **)*(pipe_counter+2));
    //cases to be handled are echo                "This is how it all began"  |    wc
    

    //write a strong parser
    /*    printf("Testing a strong parser \n");
    printf("What have i sent! Echo dolla dolla  \" dolla re \"    \n");


    parseString("         Echo     dolla        dolla  \" dolla re \"   \"Shesh is the best \" ");*/

    for(i=0;i<(pipe_counter+1);i++)
    {
        parseString(arguments[i]);
        index_3d++;
    }
    all_my_args[index_3d]=NULL;

    char **myargs[100];

/*    printf("PRINTING all the copied stuff \n");
    for(i=0;i<(pipe_counter+1);i++)
    {
        // myargs[i]=all_my_args[i];
        int j=0;
        while(all_my_args[i][j]!=NULL)
        {
            printf("%s",all_my_args[i][j] );
            j++;
        }
        if(all_my_args[i][j]==NULL)
            printf("I have handled null also");

        printf("\n");
    }
    printf("ENDED printing all the copied stuff\n");*/


//    return 0;
    //this is very importanat
    pipe_counter++;
   

    pipe_counter=index_3d;
    return all_my_args;
}



void parseString(char string[])
{
    //patterns that cause a word are "space followed by a word till word followed by space ends" =  1 word
    //in case of a double quote this is whats gonna happen as soon as you find a double quote, until you find another double quote
    //consider it as a string
    //echo "Whats up     doc?!?"
    //first lets try to print the number of words and then declare a pointer of that many number
//    printf("print the string and gtfo %s\n",string);
//    return;
    int i=0,flag=0;
    int number_of_2d=0;
    //this is to handle the spaces in the beginning
    while(string[i]==' ')
    {
        i++;
    }

    //First pressence of the letter
    while(string[i]!='\0')
    {
        if(string[i]==' ')
        {
            while(string[i]==' ' && string[i]!='\0')
            {
                i++;
            }
        }


    
       //flag=0; 
       if(string[i]!='"' && string[i]!='\'')
        {

            if(string[i]==' ')
            {
                while(string[i]==' ' && string[i]!='\0')
                {
                    i++;
                }
            }

            //print until you ecounter a space
            int j=i;
            while(string[i]!=' ' && string[i]!='\0')
            {
                i++;
            }
            //eg:              echo i=4 j=0;
            char *word;

            word=(char *)malloc(sizeof(char)*(i-j+1));
            int k=0;
            while(j!=i)
            {
                word[k]=string[j];
                j++;
                k++;
            }
            word[k]='\0';
            //printf("%s\n",word);
            number_of_2d++;
            free(word);

        }
        else if(string[i]=='"'){
//            printf("Enter the else part \n");
//          printf("--------- -%c\n",string[i]);
            int j=i;
            j++;
            i++;
            flag=1;
//            printf("-------- %c\n",string[i]);
            while(string[i]!='"' && string[i]!='\0')
            {
                i++;
            }

            char *word;
            word=(char *)malloc(sizeof(char)*(i-j+1));
            int k=0;
            while(j<i) //change is while(j<=i)
            {
                word[k]=string[j];
                j++;
                k++;
            }
            word[k]='\0';


//            printf("The string contains a double quote\" \n "); 
 //           printf("The double quoted word is %s \n",word);
//            printf("%s\n",word);
            number_of_2d++;
            free(word);
//            return ;
        
        }
       else if(string[i]=='\'')
       {
//            printf("Enter the else part \n");
//          printf("--------- -%c\n",string[i]);
            int j=i;
            //j++;
            i++;
            flag=1;
//            printf("-------- %c\n",string[i]);
            while(string[i]!='\'' && string[i]!='\0')
            {
                i++;
            }

            char *word;
            word=(char *)malloc(sizeof(char)*(i-j+1));
            int k=0;
            while(j<=i) //change is while(j<=i)
            {
                word[k]=string[j];
                j++;
                k++;
            }
            word[k]='\0';


//            printf("The string contains a double quote\" \n "); 
 //           printf("The double quoted word is %s \n",word);
//            printf("%s\n",word);
            number_of_2d++;
            free(word);
//            return ;



       }

        i++;
    }

    if(flag==1)
        number_of_2d--;

    // printf("Number of 2d is %d \n",number_of_2d);

    //declare the 2d array pointer now
    all_my_args[index_3d]=(char **)malloc((number_of_2d+1)*sizeof(char *));
    
    //reset no of 2d so that we can copy 1d array
    number_of_2d=0;
    i=0;
    
//--------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------BEGIN HERE-------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------------//
    while(string[i]==' ')
    {
        i++;
    }

    //First pressence of the letter
    while(string[i]!='\0')
    {
        if(string[i]==' ')
        {
            while(string[i]==' ' && string[i]!='\0')
            {
                i++;
            }
        }

       if(string[i]!='"' && string[i]!='\'')
        {

            if(string[i]==' ')
            {
                while(string[i]==' ' && string[i]!='\0')
                {
                    i++;
                }
            }

            //print until you ecounter a space
            int j=i;
            while(string[i]!=' ' && string[i]!='\0')
            {
                i++;
            }
            //eg:              echo i=4 j=0;
          //  char *word;
            all_my_args[index_3d][number_of_2d]=(char *)malloc(sizeof(char)*(i-j+1));
            int k=0;
            while(j!=i)
            {
                all_my_args[index_3d][number_of_2d][k]=string[j];
                j++;
                k++;
            }
            all_my_args[index_3d][number_of_2d][k]='\0';
            // printf("%s\n", all_my_args[index_3d][number_of_2d]);
            number_of_2d++;
           // free(word);

        }
        else if(string[i]=='"')
        {
//            printf("Enter the else part \n");
//          printf("--------- -%c\n",string[i]);
            
            int j=i;
            i++;
            j++;
//            printf("-------- %c\n",string[i]);
            while(string[i]!='"' && string[i]!='\0')
            {
                i++;
            }

           // char *word;
            all_my_args[index_3d][number_of_2d]=(char *)malloc(sizeof(char)*(i-j+1));
            int k=0;
            while(j<i)
            {
                all_my_args[index_3d][number_of_2d][k]=string[j];
                j++;
                k++;
            }
            all_my_args[index_3d][number_of_2d][k]='\0';

            i++;
//            printf("The string contains a double quote\" \n "); 
 //           printf("The double quoted word is %s \n",word);
            // printf("%s\n", all_my_args[index_3d][number_of_2d]);
            number_of_2d++;
//            free(word);
//            return ;
        
       }
       else if(string[i]=='\'')
       {
//            printf("Enter the else part \n");
//          printf("--------- -%c\n",string[i]);
            
            int j=i;
            i++;
           // j++;
//            printf("-------- %c\n",string[i]);
            while(string[i]!='\'' && string[i]!='\0')
            {
                i++;
            }

           // char *word;
            all_my_args[index_3d][number_of_2d]=(char *)malloc(sizeof(char)*(i-j+1));
            int k=0;
            while(j<=i)
            {
                all_my_args[index_3d][number_of_2d][k]=string[j];
                j++;
                k++;
            }
            all_my_args[index_3d][number_of_2d][k]='\0';

            i++;
//            printf("The string contains a double quote\" \n "); 
 //           printf("The double quoted word is %s \n",word);
            // printf("%s\n", all_my_args[index_3d][number_of_2d]);
            number_of_2d++;
//            free(word);
//            return ;
        

           
       }

        i++;
    }

    all_my_args[index_3d][number_of_2d]=NULL;
    // printf("The index of index_3d is %d\n",index_3d);
    // number_of_2d++;

}

