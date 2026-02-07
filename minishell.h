#ifndef MINISHELL_H
#define MINISHELL_H
#include <stddef.h>
#include <stdio.h>


extern int is_builtin_command(char* command);
extern void execute_non_builtin_commands(char* command, char** arguments);
extern char* get_instruction();
extern int tokenize(char* instruction, char* command, char** arguments);
extern void free_arguments_array(char** arguments, int number_of_arguments);

   
#endif 