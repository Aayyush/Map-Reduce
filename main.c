#include <stdio.h>
#include <stdlib.h>
#include "MapReduce.h"

#define NUMBER_OF_MAPPERS 10
#define NUMBER_OF_REDUCERS 4

char** _split(char* fileName) {
    char *temp = (char*)malloc(sizeof(12));
    sprintf(temp, "Hello World!");
    return &temp;
}

key_value* _map(char* fileName) {
    // Read file. 
    // For each word, create a new key as word, value as 1. 
    FILE *fp_in = fopen(fileName, "r");
    char* temp;
    key_value *head, *curr; 
    head = curr = NULL;
    
    int i = 0; 
    while (fscanf(fp_in, "%s", temp) > 0){
        printf("Key: %s\n", temp);
        
        if (head == NULL) {
            head = (key_value*)malloc(sizeof(key_value));;
            curr = head;
        } 
        else{
            curr->next = (key_value*)malloc(sizeof(key_value));;
            curr = curr->next;
        }
        curr->key = temp;
        curr->value = (int) i;
        curr->next = NULL;
        i++;
    }
    fclose(fp_in);
    return head;
}

void _reduce(HashMap* hash_map) {
    return;
}

int _shuffle(char* key) {
    printf("Shuffle key is %s Char is %c\n", key, *key);
    int* t = (int*)malloc(sizeof(int));
    (*t) = ((*key) - 'a' + 1) % 5;
    printf("Key is %s Reducer Index is %d\n", key, *t);
    return *t;
}

int main(){
    initialize_map_reduce(NUMBER_OF_MAPPERS, NUMBER_OF_REDUCERS, &_split, &_map, &_reduce, &_shuffle);
    int* t = (int*)malloc(sizeof(int));
    (*t) = 1;
    run_mapper(t);
    return 0;
}
