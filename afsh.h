#include "list.h"

#ifndef PEX1_AFSH_H
#define PEX1_AFSH_H

#define BUFF_MAX 4096
#define ARG_MAX 64

void history(node* histHead);
char* recall(node* histHead, char* item);
char** parseInput(char*, char**, size_t argSize);

#endif //PEX1_AFSH_H
