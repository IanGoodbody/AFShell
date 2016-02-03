#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "list.h"
#include "afsh.h"

int main(void){

    char* inputStr      = (char*) malloc(STR_MAX);
    char* cleanInputStr = (char*) malloc(STR_MAX);
    char* cwdStr        = (char*) malloc(STR_MAX);
    char* homeStr       = (char*) malloc(STR_MAX);
    char** argList      = (char**) malloc(ARG_ARRAY_MAX);
    node* historyList = NULL;

    strcpy(homeStr,  "/home/");
    if(getlogin_r(homeStr + 6, STR_MAX - 7));
        printf("Could not retrieve user login.\n");

    printf("\nWelcome to the Air Force Shell (afsh)\n\n");
    while(1) {

        printf("Prompt> ");
        fgets(inputStr, STR_MAX, stdin);
        parseInput(inputStr, argList, ARG_ARRAY_MAX);

        if(!*argList)
            continue;

        if(strcmp(*argList, "history") && strcmp(*argList, "recall")) {
            cleanInput(cleanInputStr, argList);
            historyList = list_remove(historyList, cleanInputStr);
            historyList = list_insert_head(historyList, cleanInputStr);
        }

        if(execArgs(argList, historyList, cwdStr, homeStr)) {
            free(inputStr);
            free(cleanInputStr);
            free(argList);
            free(cwdStr);
            free(homeStr);
            list_destroy(historyList);
            exit(0);
        }
    }
}

int execArgs(char** argList, node* historyList, char* cwdStr, char *homeStr){
    if(!*argList)
        return 0;
    else if (!strcmp(*argList, "history")) {
        printf("\nCommand History:\n");
        // Set the iteration length
        int i = list_length(historyList); // longest possible list length

        if(!*(argList+1)){
            if(i > 10)
                i = 10;
        }
        else if( i > atoi(*(argList+1))) // Use user specified if there more than that number to show
            i = atoi(*(argList+1));

        for( ; i > 0; i--){
            printf("%i.\t", i);
            list_printn(historyList, i);
        }
        return 0;
    }
    else if (!strcmp(*argList, "recall")) {
        int index;
        char* recallStr;

        if(*(argList+1))
            index = atoi(*(argList+1));
        else
            index = 1;
        // Check that the list returned a value
        if((recallStr = list_get(historyList, index))) {
            printf("Executing: %s:\n", recallStr);
            parseInput(recallStr, argList, ARG_ARRAY_MAX);
            execArgs(argList, historyList, cwdStr, homeStr);
        }
        return 0;
    }
    else if (!strcmp(*argList, "cd")) {
        if(!strcmp(*(argList+1), "~"))
            chdir(homeStr);
        else
            chdir(*(argList+1));
        printf("Directory changed to: %s\n", getcwd(cwdStr, STR_MAX));
        return 0;
    }
    else if (!strcmp(*argList, "exit")) {
        return 1;
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
        return 0;
    }
}

char* cleanInput(char* nString, char** argArray){
    char* sString = nString;
    while(*argArray){
        int i = 0;
        while(*(*argArray + i)){
            *nString++ = *(*argArray+i);
            i++;
        }
        *nString++ = ' ';
        argArray++;
    }
    *nString = '\0';
    return sString;
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
                    if((argArray - saveArray) * sizeof(char*) < argSize - 1) // Ensure Args buffer won't overflow
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

