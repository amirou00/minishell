#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <unistd.h> // shows error squiggles, but it's fine
#include <sys/wait.h> // same
#define MAX_INSTRUCTION_SIZE 100 //an arbitrary value
#define MAX_COMMAND_SIZE 50 //an arbitrary value
#define MAX_ARGUMENTS_SIZE 50 //an arbitrary value 
#define NON_BUILTIN_COMMANDS_NUMBER 2


struct token{
    char command[MAX_COMMAND_SIZE];
    char* args[MAX_ARGUMENTS_SIZE];
};


/*void free_arguments_array(char** arguments, int number_of_arguments){
    for(int i = 0; i < number_of_arguments; i++){
        free(arguments[i]); 
    }
}*/


void free_arguments_array(char** arguments, int number_of_arguments){
    if(arguments == NULL) return;
    
    // Free all allocated arguments including the command (index 0)
    for(int i = 0; i < number_of_arguments; i++){
        if(arguments[i] != NULL) {
            free(arguments[i]); 
        }
    }
}


char* non_builtin_commands[] = {"cd", "exit"};


int is_non_builtin_command(char* command){
    for(int i = 0; i < NON_BUILTIN_COMMANDS_NUMBER; i++){
        if(strcmp(command, non_builtin_commands[i]) == 0){
            return 1; //true
        }
    }
    return 0; //false
}


void execute_builtin_commands(char* command, char** arguments){
    pid_t pid = fork(); //again, here it shows error squiggles, but it does not bug during the compilation
    if(pid == 0){
        // child process
        execvp(command, arguments);
        perror("problem with the execution");
        exit(127);
    }
    else{
        // parent process
        wait(NULL);
        printf("\n");
    }
}


char* get_instruction(){
    printf("user :");
    fflush(stdout);
    char* instruction = malloc(MAX_INSTRUCTION_SIZE * sizeof(char)); // freed in main
    if(fgets(instruction, MAX_INSTRUCTION_SIZE, stdin) == NULL){
        perror("technical problem occured");
        return NULL;
    }
    size_t size = strlen(instruction);
    if(instruction[size - 1] == '\n'){  
        instruction[size - 1] = '\0'; 
    }
    return instruction;
}


/*int tokenize(char* instruction, char* command, char** arguments){
    struct token my_token; 
    char auxiliary_var[MAX_INSTRUCTION_SIZE];
    int auxiliary_var_index = 0;
    int args_index = -1; 
    int i = 0;
    while(instruction[i] != '\0'){
        while(instruction[i] == ' '){
            i++;
        }
        while(instruction[i] != ' ' && instruction[i] != '\0'){
            auxiliary_var[auxiliary_var_index] = instruction[i];
            auxiliary_var_index++;
            i++;
        }
        auxiliary_var[auxiliary_var_index] = '\0';
        if(args_index < 0){
            strcpy(my_token.command, auxiliary_var);
            args_index++;
            my_token.args[args_index] = malloc(sizeof(auxiliary_var)); // note sure..
            strcpy(my_token.args[args_index], auxiliary_var);
            args_index++;
        }
        else{
            my_token.args[args_index] = malloc(sizeof(auxiliary_var)); // note sure..
            strcpy(my_token.args[args_index], auxiliary_var); 
            args_index++;
        }
        auxiliary_var[0] = '\0';            
        auxiliary_var_index = 0;
        while(instruction[i] == ' '){
            i++;
        }
    }
    my_token.args[args_index] = NULL;

    command = my_token.command;
    arguments = my_token.args; 

    return args_index;
}*/


int tokenize(char* instruction, char* command_ptr, char** arguments_ptr){
    // We'll use a static struct to hold the tokens
    static struct token my_token; 
    char auxiliary_var[MAX_INSTRUCTION_SIZE];
    int auxiliary_var_index = 0;
    int args_index = 0;  // Changed from -1 to 0
    int i = 0;
    
    // Initialize the first arg pointer to NULL
    my_token.args[0] = NULL;
    
    while(instruction[i] != '\0'){
        // Skip leading spaces
        while(instruction[i] == ' '){
            i++;
        }
        
        // Check if we're at the end after skipping spaces
        if(instruction[i] == '\0'){
            break;
        }
        
        // Read token
        auxiliary_var_index = 0;
        while(instruction[i] != ' ' && instruction[i] != '\0'){
            auxiliary_var[auxiliary_var_index] = instruction[i];
            auxiliary_var_index++;
            i++;
        }
        auxiliary_var[auxiliary_var_index] = '\0';
        
        if(args_index == 0){
            // First token is the command
            strcpy(my_token.command, auxiliary_var);
            my_token.args[args_index] = malloc(strlen(auxiliary_var) + 1);
            if(my_token.args[args_index] == NULL) {
                // Cleanup on error
                for(int j = 0; j < args_index; j++) {
                    free(my_token.args[j]);
                }
                return -1;
            }
            strcpy(my_token.args[args_index], auxiliary_var);
            args_index++;
        }
        else{
            // Subsequent tokens are arguments
            my_token.args[args_index] = malloc(strlen(auxiliary_var) + 1);
            if(my_token.args[args_index] == NULL) {
                // Cleanup on error
                for(int j = 0; j < args_index; j++) {
                    free(my_token.args[j]);
                }
                return -1;
            }
            strcpy(my_token.args[args_index], auxiliary_var); 
            args_index++;
        }
        
        // Skip trailing spaces
        while(instruction[i] == ' '){
            i++;
        }
    }
    my_token.args[args_index] = NULL;
    
    // Return pointers to caller
    command_ptr = my_token.command;
    arguments_ptr = my_token.args; 
    
    return args_index;  // Number of arguments (including command)
}