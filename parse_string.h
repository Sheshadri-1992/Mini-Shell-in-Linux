#ifndef PARSE_H_
#define PARSE_H_

extern int pipe_counter;
extern char **all_my_args[1000];
char*** parse_function(char command[]);
void parse_string(char string[]);
int return_length();
#endif
