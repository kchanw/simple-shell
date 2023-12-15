/**************************************************************
* Class:  CSC-415-01 Fall 2022
* Name:Kurtis Chan
* Student ID:918319175 
* GitHub ID:kchanw
* Project: Assignment 3 – Simple Shell
*
* File: Chan_Kurtis_HW3_main.c
*
* Description: Writing a simple shell by using fork() and execvp() to handle commands.
* must take in a line of text and tokenize it to be ready for execvp(). Delim by space, tab, newline(whitespaces)
* Need to handle EOF and blank spaces.
*
**************************************************************/


 #include <stdlib.h>
 #include <stdio.h>
 #include <unistd.h>
 #include <string.h>
 #include <sys/wait.h>


 int main(int argc,char *argv[]){
    size_t bufferSize = 123;
    int size = 16;
    char* prompt = ">";

    if(argv[1] != NULL){//prompt entered by user
        prompt = argv[1];
    }


    while(1){ //shell loop
    printf("%s", prompt);//prompt
    char*buffer = malloc(bufferSize * sizeof(char));//allocate memory for 123 chars/ bytes
    getline(&buffer, &bufferSize,stdin);//from linuxhint.com/getline-function-c/
    //whatever is on the line up to 123 chars is stored in buffer


    pid_t res;//for the ids later, child and for cases.
    int i = 0;

    if(feof(stdin)){//handling EOF, stdin is the FILE*stream from getline 
        printf("\n");
        exit(0);
    }

    char delim[] = " \t\n";
    //char *command = strtok(buffer, delim);//getting the command to run
    char **argList = malloc(bufferSize* sizeof(char*));//list of arguments for execvp() 
    
    char *holder = strtok(buffer,delim);

    while(holder != NULL){//go through strtok till it hits null to get the list of arguments
        argList[i] = holder;
        //printf("%s\n", holder);
        holder = strtok(NULL, delim);
        i++;
    }
    argList[i] = NULL;


            switch((res = fork())){// since child is 0, it will only run if it is the child
        
                case 0: {//child case
                if(execvp(argList[0], argList)){//second argument skips first index
                    if(strcmp(argList[0],"exit") != 0){//so no error output when exiting
                    perror("ERROR");
                    }
            }
            }
                case -1 :{//error case
                //printf("Error %d\n", res);
                //return res;
                exit(1);
            }
                default : {//parent case
                //pid_t waiting;
                int status;
                wait(&status);//waiting for child
                //from man7.org/linux/man-pages/man2/wait.2.html

                if(argList[0] != NULL){
                    if(strcmp(argList[0], "exit") == 0){//exit when exit is inputted and not blank line
                    exit(0);
                    } 
                }

                printf("Child %d  ", res);
                printf(", exited with %d\n", status);


            }
        }
    argList= NULL;
    free(argList);
    buffer = NULL;
    free(buffer);//setting pointers to null and freeing the allocated memory
    }
}