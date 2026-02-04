#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <string.h>
#include <unistd.h> //for whatever reason this freak wants to stay red, even though i spent the entire day undoing it
//works though, no compilation error, no execution error, yet...
#include <sys/wait.h>
#define MAX_COMMAND_SIZE 100 //an arbitrary value

int main(){

    while(1){
        printf("user :");
        fflush(stdout);
        char command[MAX_COMMAND_SIZE];
        if(fgets(command, MAX_COMMAND_SIZE, stdin) == NULL){
            perror("technical problem occured");
            break;
        }
        size_t size = strlen(command);
        if(command[size - 1] == '\n'){
           command[size - 1] = '\0'; 
        }
        if(strcmp(command, "exit") == 0){
            break;
        }
        system(command);
    }
    return 0;
}








/*      char buffer;
        printf("user : ");
        fflush(stdout);
        buffer = getchar();
        char* command = malloc(MAX_COMMAND_SIZE * sizeof(char));
        for(int i = 0; (i < MAX_COMMAND_SIZE) && buffer != '\n'; i++){
            command[i] = buffer;
            buffer = getchar();
        }
        if(strcmp(command, "exit") == 0){
            break;
        }
        system(command);
        printf("\n");    */