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
        if(cNode){     //cNode will be null if malloc cannot allocate the, non-null value is valid
            cNode->next = NULL;
            char *nData;
            nData = (char *) malloc(sizeof(char) * (strlen(data) + 1)); //I am loyal to ANSI C for no good reason
            if(nData)
                cNode->data = strcpy(nData, data);
            else{
                printf("Memory for the node string was not allocated, node creation failed.\n");
                free(cNode);
                cNode = NULL;
            }
        }
        else
            printf("Memory could not be allocated for the new node. Node creation failed.\n");
    }
    return cNode;
}

node* list_insert_head(node* cNode, char* data){
    node* nNode = (node*) malloc(sizeof(node));
    if(nNode) {
        nNode->next = cNode;
        char *nData = (char *) malloc(sizeof(char) * (strlen(data) + 1));
        if(nData){
            nNode->data = strcpy(nData, data);
            return nNode;
        }
        else{
            printf("Memory could not be allocated for the node data, node creation failed.\n");
            free(nNode);
            nNode = NULL;
            return cNode;
        }
    }
    else {
        printf("Memory could not be allocated for tne new node. Node creation failed.\n");
        return cNode;
    }
}

node* list_insertn(node* cNode, char* data, int index){
    index--;
    if(index < 0) { // Index was given as zero or negative
        printf("Index out of bounds Dunderkompf.\n");
        return cNode;
    }
    else if(!cNode && index) { // Index is out of bounds
        printf("Index is outof bounds.\n");
        return cNode;
    }
    else if(index){
        cNode->next=list_insertn(cNode->next, data, index);
        return cNode;
    }
    else{
        node* nNode = (node*) malloc(sizeof(node));
        if(nNode) {
            nNode->next = cNode;
            char *nData = (char *) malloc(sizeof(char) * (strlen(data) + 1));
            if(nData) {
                nNode->data = strcpy(nData, data);
                return nNode;
            }
            else{
                printf("memory could not be allocated for the new node data. Node creation failed.\n");
                free(nNode);
                nNode = NULL;
                return cNode;
            }
        }
        else {
            printf("Memory could not be allocated for the node. Node creation failed.\n");
            return cNode;
        }
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
        cNode->next = NULL;
        free(cNode);
        return sNode;
    }
    else{
        cNode->next = list_remove(cNode->next, targetData);
        return cNode;
    }
}

node* list_removen(node* cNode, int index){
    index--;
    if(index < 0) { // Index was negative or zero
        printf("Index out of bounds Dunderkompf.\n");
        return NULL;
    }
    else if(!cNode) { //index exceeded the end of the list
        printf("Index out of bounds.\n");
        return cNode;
    }
    else if(!index){ // index is zero remove this node
        node* sNode = cNode->next;
        free(cNode->data);
        cNode->next = NULL;
        free(cNode);
        return sNode;
    }
    else{ // Recursive case
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

void list_printRF(node* cNode){
   list_printRF_helper(cNode, 1);
}
void list_printRF_helper(node* cNode, int index){
   if(cNode){
       list_printRF_helper(cNode->next, index+1);
       printf("%i.\t", index);
       printf("%s\n", cNode->data);
   }
}

void list_printn(node* cNode, int index){
    index--;
    if(index < 0){
        printf("Index out of bounds Dunderkompf.\n");
    }
    else if(cNode){
        if (!index)
            printf("%s\n", cNode->data);
        else
            list_printn(cNode->next, index);
    }
}

char* list_get(node* cNode, int index){
    index--;
    if(index < 0) // Index negative or zero
        return NULL;
    else if(!cNode) // Index exceeds the end of the list
        return NULL;
    else if(!index) // Return this node's data.
        return cNode->data;
    else // Recursive case
        return list_get(cNode->next, index);
}

node* list_destroy(node* cNode){
    if(cNode) {
        list_destroy(cNode->next);
        free(cNode->data);
        cNode->next = NULL;
        free(cNode);
    }
    return NULL;
}
