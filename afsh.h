#include "list.h"

#ifndef PEX1_AFSH_H
#define PEX1_AFSH_H

#define STR_MAX sizeof(char) * 1024
#define ARG_ARRAY_MAX sizeof(char*) * 64

/*
 * execArgs: Executes the arguments in argList. Seperate function allows for recursive use of recall.
 * Parameters:
 *  - argList: char** a NULL terminated array of string pointers
 *  - historyList: node* pointer to the head of the linked list storing the history of all commands
 *      Note that because the pointers is pass-by-value the list cannot be changed in this function
 *  - cwdStr: char* The buffer where the program stores the current directory
 *  Returns: 1 as the exit condition, 0 for the continue condition
 */
int execArgs(char** argList, node* historyList, char* cwdStr);

/*
 * cleanInput: constructs a cleanly formatted string from an arguments array
 * Parameters:
 *  - nString: char* a pointer to the buffer where the cleaned up string is stored
 *  - argArray: char** pointer to the arguments list
 * Returns: char* pointer to the formatted string
 */
char* cleanInput(char* nString, char** argArray);

/*
 * parseInput: converts the raw console stirng into an array of arguments that can be read by the interpreter
 *  The args list created by this function is an array containing pointers to the data in the input buffer. The
 *  data in the buffer is injected with null values in order to seperate the string. Therefor argArray will be invalid
 *  after the inBuff is reset.
 * Parameters:
 *  - inBuff: char* pointer to the raw string
 *  - argArray: char** pointer to the arguments list where the pointers will be stored
 *  - argSize: size_t the size of the array in bytes
 *  Returns: char** pointer to the list of arguments.
 */
char** parseInput(char* inBuff, char** argArray, size_t argSize);

#endif //PEX1_AFSH_H
