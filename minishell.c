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

int main(){

    struct token{
        char command[MAX_COMMAND_SIZE];
        char* args[MAX_ARGUMENTS_SIZE];
    };

    while(1){
        // get the instruction phase
        printf("user :");
        fflush(stdout);
        char instruction[MAX_INSTRUCTION_SIZE];
        if(fgets(instruction, MAX_INSTRUCTION_SIZE, stdin) == NULL){
            perror("technical problem occured");
            break;
        }

        //parse the instruction phase
        size_t size = strlen(instruction);
        if(instruction[size - 1] == '\n'){
            instruction[size - 1] = '\0'; 
        }
        if(strcmp(instruction, "exit") == 0){
            break;
        }  
        struct token my_token; 
        char auxiliary_var[MAX_INSTRUCTION_SIZE];
        int auxiliary_var_index = 0;
        int args_index = -1; 
        int i = 0;
        while(i < size){
            while(instruction[i] == ' '){
                i++;
            }
            while(instruction[i] != ' '){
                auxiliary_var[auxiliary_var_index] = instruction[i];
                auxiliary_var_index++;
                i++;
            }
            auxiliary_var[auxiliary_var_index] = '\0';
            if(args_index < 0){
                strcpy(my_token.command, auxiliary_var);
            }
            else{
                my_token.args[args_index] = malloc(sizeof(auxiliary_var));
                strcpy(my_token.args[args_index], auxiliary_var);
            }
            auxiliary_var[0] = '\0';            
            auxiliary_var_index = 0;
            args_index++;
            while(instruction[i] == ' '){
                i++;
            }
        }
        my_token.args[args_index] = NULL;
        
        //the important part
        pid_t pid = fork(); //again, here it shows error squiggles, but it does not bug during the compilation
        if(pid == 0){
            execvp(my_token.command, my_token.args);
            perror("problem with the execution");
            exit(127);
        }
        else{
            wait(NULL);
        }

        // !!! do not forget to free*
        for(int i = 0; i < args_index; i++){
            free(my_token.args[i]); 
        }
    }
    return 0;
}
