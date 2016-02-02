#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "list.h"
#include "afsh.h"

int main(void){

    char* inputBuff = (char*) malloc(BUFF_MAX);
    char** argList = (char**) malloc(sizeof(char*) * ARG_MAX);
    node* historyList = NULL;

    printf("Welcome to the Air Force Shell (afsh)\n\n");
    while(1) {

        printf("Prompt> ");
        fgets(inputBuff, BUFF_MAX, stdin);
        historyList = list_insert_head(historyList, inputBuff);
        parseInput(inputBuff, argList, ARG_MAX);

        if (!strcmp(*argList, "history")) {
            int i;
            if(!*(argList+1)){
                for(i = 10; i > 0; i--) {
                    printf("%i.\t", i);
                    list_printn(historyList, i);
                }
            }
            else{
                for(i = atoi(*(argList+1)); i > 0; i--){
                    printf("%i.\t", i);
                    list_printn(historyList, i);
                }
            }
        }
        else if (!strcmp(*argList, "recall")) {

        }
        else if (!strcmp(*argList, "cd")) {

        }
        else if (!strcmp(*argList, "exit")) {
            free(inputBuff);
            free(argList);
            historyList = list_destroy(historyList);
            exit(0);
        }
        else {
            pid_t forkedID = fork();
            if (forkedID < 0)
                printf("Fork failed.\n");
            else if (forkedID > 0) {
                wait(NULL);
            }
            else {
                execvp(*argList, argList);
                exit(0);
            }
        }
    }
}

char** parseInput(char* inBuff, char** argArray, size_t argSize){
    // Use a finite state machine to break the input string into a useful set of tokens.
    // WHITESPACE will pass over and "remove" series of whitespace characters.
    // When it encounters a character it changes the state to READ_ARG and sets a pointer in the arg array to
    //  the first token of the word
    // READ_ARG iterates through non-whitespace characters looking for the end of the argument.
    // When READ_ARG reaches a whitespace character it sets that whitespace to NULL to terminate that section
    //  of the string, and changes the state back to WHITESPACE.
    // ECE 383 for the win!

    enum state {WHITESPACE, READ_ARG};
    enum state parserState = WHITESPACE;
    char** saveArray = argArray;

    while(*inBuff){
        switch (parserState) {
            case WHITESPACE:
                if(!isspace(*inBuff)){
                    parserState = READ_ARG;
                    *argArray++ = inBuff;
                }
                inBuff++;
                break;
            case READ_ARG:
                if(isspace(*inBuff)){
                    parserState = WHITESPACE;
                    *inBuff = '\0';
                    if(argArray - saveArray < argSize - 1) // Ensure Args buffer won't overflow
                        inBuff++;
                }
                else
                    inBuff++;
                break;
        }
    }
    *argArray = NULL;
    return saveArray;
}

