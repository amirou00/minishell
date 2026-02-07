#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <unistd.h> // shows error squiggles, but it's fine
#include <sys/wait.h> // same
#define MAX_INSTRUCTION_SIZE 100 //an arbitrary value
#define BUILTIN_COMMANDS_NUMBER 2


void free_arguments_array(char** arguments, int number_of_arguments){
    if(arguments == NULL) return;
    
    // Free all allocated arguments including the command (index 0)
    for(int i = 0; i < number_of_arguments; i++){
        if(arguments[i] != NULL) {
            free(arguments[i]); 
        }
    }
}


char* builtin_commands[] = {"cd", "exit"};


int is_builtin_command(char* command){
    for(int i = 0; i < BUILTIN_COMMANDS_NUMBER; i++){
        if(strcmp(command, builtin_commands[i]) == 0){
            return 1; //true
        }
    }
    return 0; //false
}


void execute_non_builtin_commands(char* command, char** arguments){
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


int tokenize(char* instruction, char* command, char** arguments){
    char auxiliary_var[MAX_INSTRUCTION_SIZE];
    int auxiliary_var_index = 0;
    int args_index = 0;
    int i = 0;
    while(instruction[i] != '\0'){
        while(instruction[i] == ' '){ // Skip spaces
            i++;
        }  
        if(instruction[i] == '\0'){
            break;
        }
        auxiliary_var_index = 0;
        while(instruction[i] != ' ' && instruction[i] != '\0'){
            auxiliary_var[auxiliary_var_index] = instruction[i];
            auxiliary_var_index++;
            i++;
        }
        auxiliary_var[auxiliary_var_index] = '\0';
        // Allocate directly into the arguments array
        arguments[args_index] = malloc(strlen(auxiliary_var) + 1);
        if(arguments[args_index] == NULL){
            for(int j = 0; j < args_index; j++){
                free(arguments[j]);
            }
            perror("memory allocatio problem");
            return -1;
        }
        strcpy(arguments[args_index], auxiliary_var);
        // The first token is also our command
        if(args_index == 0) {
            strcpy(command, auxiliary_var);
        }
        args_index++;
    }
    arguments[args_index] = NULL; // NULL terminate for execvp
    return args_index;
}