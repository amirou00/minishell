#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <unistd.h> //for whatever reason this freak wants to stay red, even though i spent the entire day undoing it
//works though, no compilation error, no execution error, yet...
#include <sys/wait.h>
#define MAX_INSTRUCTION_SIZE 100 //an arbitrary value
#define MAX_COMMAND_SIZE 50 //an arbitrary value
#define MAX_ARGUMENTS_SIZE 50 //an arbitrary value
#define NON_BUILTIN_COMMANDS_NUMBER 2



int main(){ 

    while(1){
        // get the instruction phase
        char* instruction = get_instruction();
        printf("1\n");
        while(instruction == NULL){
            instruction = get_instruction();
            printf("2\n");
        }
        printf("3\n");

        if(strcmp(instruction, "exit") == 0){
            break;
        }  

        //parse the instruction phase
        char command[MAX_COMMAND_SIZE];
        char* arguments[MAX_ARGUMENTS_SIZE];
        printf("4\n");
        int number_of_arguments = tokenize(instruction, command, arguments);
        printf("5\n");

        //chosk for cd
        //if(strcmp(my_token.command, "cd") == 0){
        //    chdir(my_token.args[1]);
        //}
        
        //the important part
        if(is_non_builtin_command(command) == 0){
            execute_builtin_commands(command, arguments);
        }
        else{   
            continue;
            // yeahhhh i have gor to do something about this part...
        }
        printf("6\n");
        free(instruction);
        free_arguments_array(arguments, number_of_arguments);
        printf("7\n");
    }
    return 0;
}
 