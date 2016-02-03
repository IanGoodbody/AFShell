#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "list.h"
#include "afsh.h"

/*
 * int main: Starts and runs the Air Force Shell.
 * Author: C1C Ian Goodbody
 * Date: 2 Feb 2015
 *
 * Documentation:
 *  - I pulled heavily from "The C programming Language" for library references, language constructs, and order
 *      of operations.  It's infuence was used throughout the project
 *  - The Online documentation for the "unistd.h" library was used as a reference for the "chdir", "getcwd", "fork",
 *      and "execvp" commands.
 *  - I referenced the Ubuntu community help forum to research environment variables
 */
int main(void){

    // Allocate memory for function buffers and objects
    char* inputStr      = (char*) malloc(STR_MAX);
    char* cleanInputStr = (char*) malloc(STR_MAX);
    char* cwdStr        = (char*) malloc(STR_MAX);
    char** argList      = (char**) malloc(ARG_ARRAY_MAX);
    node* historyList = NULL;

    printf("\nWelcome to the Air Force Shell (afsh)\n\n");
    while(1) {

        // Get input from user
        printf("Prompt> ");
        fgets(inputStr, STR_MAX, stdin);
        parseInput(inputStr, argList, ARG_ARRAY_MAX);

        // Check for no command entered, skip the rest of the loop and do not process anything.
        if(!*argList)
            continue;

        // If the commands are not "history", "exit", or "recall" add the command to the history list
        if(strcmp(*argList, "history") && strcmp(*argList, "recall") && strcmp(*argList, "exit")) {
            cleanInput(cleanInputStr, argList);
            historyList = list_remove(historyList, cleanInputStr);
            historyList = list_insert_head(historyList, cleanInputStr);
        }

        //  Execute the argument in the list, check for exit condition
        if(execArgs(argList, historyList, cwdStr)) {
            // if execArgs returns 1 for exit clear allocated memory and exit the program
            free(inputStr);
            free(cleanInputStr);
            free(argList);
            free(cwdStr);
            list_destroy(historyList);
            exit(0);
        }
    }
}

int execArgs(char** argList, node* historyList, char* cwdStr){

    // Process "history" command
    if (!strcmp(*argList, "history")) {
        printf("\nCommand History:\n");
        // Set the maximum iteration length
        int i = list_length(historyList);
        // User entered no lengh command, set default to 10 or longest possible
        if(!*(argList+1)){
            if(i > 10)
                i = 10;
        }
        else if( i > atoi(*(argList+1))) // set to user specified length or the logest possible
            i = atoi(*(argList+1));

        // Iteratively print the list tail to head and add numbers
        for( ; i > 0; i--){
            printf("%i.\t", i);
            list_printn(historyList, i);
        }
        return 0;
    }

    // process the "recall" command
    else if (!strcmp(*argList, "recall")) {
        int index; // The index of the command to be recalled
        char* recallStr; // Command recalled from the list

        if(*(argList+1))
            index = atoi(*(argList+1)); // Set index to the user input
        else
            index = 1; // Default is index 1

        // Check that the list returned a value
        if((recallStr = list_get(historyList, index))) {
            printf("Executing: %s:\n", recallStr);
            // Execute the new command
            parseInput(recallStr, argList, ARG_ARRAY_MAX);
            execArgs(argList, historyList, cwdStr);
        }
        return 0;
    }
    // process the "cd"
    else if (!strcmp(*argList, "cd")) {
        // User no directory, just move on
        if(!*(argList+1));
        // process the "home" call
        else if(!strcmp(*(argList+1), "~"))
            chdir(getenv("HOME"));
        // Process user defined call
        else {
            // check if chdir returns an error
            if(chdir(*(argList + 1)))
                printf("Directory does not exist.\n");
        }
        printf("cwd=> %s\n", getcwd(cwdStr, STR_MAX));
        return 0;
    }
    // Process the "exit" command
    else if (!strcmp(*argList, "exit")) {
        return 1;
    }
    // process not recognized, pass the command on to the system.
    else {
        pid_t forkedID = fork();
        if (forkedID < 0)
            printf("Fork failed\n");
        else if (forkedID > 0) {
            wait(NULL);
        }
        else {
            // Check if the system call returned an error
            if(execvp(*argList, argList))
                printf("Command not found\n");
            exit(0);
        }
        return 0;
    }
}

char* cleanInput(char* nString, char** argArray){
    char* sString = nString; // Save the pointer to the head of the string to be returned
    while(*argArray){
        int i = 0;
        while(*(*argArray + i))
            *nString++ = *(*argArray+i++); // copy the data from argArray to nString with the power of C
        *nString++ = ' '; // Add only one space between each argument
        argArray++;
    }
    *nString = '\0'; // Terminate teh string
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

