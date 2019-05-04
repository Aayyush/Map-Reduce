#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "MapReduce.h"

#define NUMBER_OF_MAPPERS 10
#define NUMBER_OF_REDUCERS 4

char** _split(char* fileName) {
    char **temp = (char**)malloc(sizeof(char*));
    *temp = (char*)malloc(12);
    sprintf(*temp, "Hello World!");
    return *temp;
}

key_value* _map(char* fileName) { 
    FILE *fp_in = fopen(fileName, "r");
    char *temp;
    
    // Pointers for the linked list. 
    key_value *head, *curr; 
    head = curr = NULL;
    
    // Counter for value in the tuple. 
    int i = 0;
    
    // Read the file until end of file. 
    while (fscanf(fp_in, "%s", temp) > 0){
        
        // Make a copy of the string read from the file. 
        // All the nodes in the list point to the same string 
        // if we don't make a copy because temp is a pointer
        // which does not change. 
        char *copy = (char*)malloc(sizeof(strlen(temp)));
        strcpy(copy, temp);
        
        if (head == NULL) {
            head = (key_value*)malloc(sizeof(key_value));;
            curr = head;
        } 
        else{
            curr->next = (key_value*)malloc(sizeof(key_value));;
            curr = curr->next;
        }
        curr->key = copy;
        
        // Assign the pointer to an integer on heap. 
        int **t = (int**)malloc(sizeof(int*));
        *t = (int*)malloc(sizeof(int));
        
        // Increment the counter. 
        **t = ++i;
        curr->value = *t;
        curr->next = NULL;
    }
    
    // Close file after reading. 
    fclose(fp_in);
    
    return head;
}

key_value* _reduce(char* key, LinkedList* list) {
    key_value* kv = calloc(1, sizeof(kv));
    kv->key = key;
    
    int* result = calloc(1, sizeof(int));
    LinkedList* temp_list = list;
    for (; temp_list; temp_list = temp_list->next){
        *result = *result + temp_list->value;
    }
    kv->value = (void *)result;
    return kv;
}

int _shuffle(char* key) {
    int t = (tolower(*key) - 'a' + 1) % NUMBER_OF_REDUCERS;
    printf("To Reducer %d\n", t);
    return t;
}

int main(){
    initialize_map_reduce(NUMBER_OF_MAPPERS, NUMBER_OF_REDUCERS, &_split, &_map, &_reduce, &_shuffle);
     
    // Run mapper called for each NUMBER_OF_MAPPERS, with the mapper_index given by the split 
    // function. Work around for now.
    int* t = (int*)malloc(sizeof(int));
    (*t) = 1;
    //run_mapper(t);
    run_reducer(t);
    return 0;
}
