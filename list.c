#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include <string.h>

node* list_insert_tail(node* cNode, char* data){
    if(cNode){
        cNode->next = list_insert_tail(cNode->next, data);
    }
    else{
        cNode = (node*) malloc(sizeof(node));
        cNode->next = NULL;
        char* nData = (char*) malloc(sizeof(char)*(strlen(data)+1)); //I am loyal to ANSI C for no good reason
        cNode->data = strcpy(nData, data);
    }
    return cNode;
}

node* list_insert_head(node* cNode, char* data){
    node* nNode = (node*) malloc(sizeof(node));
    nNode->next = cNode;
    char* nData = (char*) malloc(sizeof(char)*(strlen(data)+1));
    nNode->data = strcpy(nData, data);

    return nNode;
}

node* list_insertn(node* cNode, char* data, int index){
    index--;
    if(index < 0)
        return NULL;
    else if(!cNode && index) //Index is out of the list range
        return cNode;
    else if(index){
        cNode->next=list_insertn(cNode->next, data, index);
        return cNode;
    }
    else{
        node* nNode = (node*) malloc(sizeof(node));
        nNode->next = cNode;
        char* nData = (char*) malloc(sizeof(char)*(strlen(data)+1));
        nNode->data = strcpy(nData, data);
        return nNode;
    }
}

int list_length(node* cNode) {
    if (cNode)
        return list_length(cNode->next)+1;
    else
        return 0;
}

node* list_remove(node* cNode, char* targetData){
    if(!cNode)
        return cNode;
    else if(!strcmp(cNode->data, targetData)){
        node* sNode = cNode->next;
        free(cNode->data);
        free(cNode);
        cNode = NULL;
        return sNode;
    }
    else{
        cNode->next = list_remove(cNode->next, targetData);
        return cNode;
    }
}

node* list_removen(node* cNode, int index){
    index--;
    if(index < 0)
        return NULL;
    else if(!cNode) //index excedes the end of the list
        return cNode;
    else if(!index){
        node* sNode = cNode->next;
        free(cNode->data);
        free(cNode);
        cNode = NULL;
        return sNode;
    }
    else{
        cNode->next = list_removen(cNode->next, index);
        return cNode;
    }
}

void list_print(node* cNode){
    if(cNode){
        printf("%s\n", cNode->data);
        list_print(cNode->next);
    }
}

void list_printn(node* cNode, int index){
    index--;
    if(cNode && index > 0) {
        if (!index)
            printf("%s\n", cNode->data);
        else
            list_printn(cNode->next, index);
    }
}

char* list_get(node* cNode, int index){
    index--;
    if(index < 0)
        return NULL;
    else if(!cNode)
        return NULL;
    else if(!index)
        return cNode->data;
    else
        return list_get(cNode->next, index);
}

void list_destroy(node* cNode){
    if(cNode) {
        list_destroy(cNode->next);
        free(cNode->data);
        free(cNode);
        cNode = NULL;
    }
}
