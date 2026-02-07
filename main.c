#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <unistd.h> // shows error squiggles, but it's fine
#include <sys/wait.h> // same
#define MAX_COMMAND_SIZE 50 //an arbitrary value
#define MAX_ARGUMENTS_SIZE 50 //an arbitrary value


int main(){ 

    while(1){
        // get the instruction phase
        char* instruction = get_instruction();
        while(instruction == NULL){
            instruction = get_instruction();
        }  
        //parse the instruction phase
        char command[MAX_COMMAND_SIZE];
        char* arguments[MAX_ARGUMENTS_SIZE];
        int number_of_arguments = tokenize(instruction, command, arguments);
        //chosk for cd
        //if(strcmp(my_token.command, "cd") == 0){
        //    chdir(my_token.args[1]);
        //}
        //the important part
        if(is_builtin_command(command) == 0){
            execute_non_builtin_commands(command, arguments);
        }
        else{   
            if(strcmp(command, "exit") == 0){
                free(instruction);
                free_arguments_array(arguments, number_of_arguments);
                break;
            }
            else if(strcmp(command, "cd") == 0){
                chdir(arguments[1]);
            }
        }
        free(instruction);
        free_arguments_array(arguments, number_of_arguments);
    }
    return 0;
}
 