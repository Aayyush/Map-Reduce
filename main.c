#include <stdio.h>
#include <stdlib.h>
#include "MapReduce.h"
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>

#define NUMBER_OF_MAPPERS 10
#define NUMBER_OF_REDUCERS 10


void _split(char* filename, int num_splits){
    FILE* fp;
    fp = fopen(filename, "r");

    fseek(fp, 0, SEEK_END); // seek to end of file.
    int size = ftell(fp); // get current file pointer.
    fseek(fp, 0, SEEK_SET); // seek to the beginning of the file.

    int split_at = floor(size / num_splits);
    
    printf("Split at %d.\n", split_at);
    int file_num = 0;
    char curr_str[500];
    FILE* curr_file = NULL;
    
    int curr_file_size = 0;  // Used to calculate the split point.

    while (fscanf(fp, "%s", curr_str) > 0){
        if ((strlen(curr_str) + 1) > split_at) {
            fprintf(stderr, "One (or more) string cannot fit into a file with the number of splits.\n");
            exit(1);
        }
        
        if (curr_file_size >= split_at || curr_file == NULL){
            // Close the file opened earlier.
            if (curr_file != NULL){
                fclose(curr_file);
            }
            
            char map_filename[50];
            sprintf(map_filename, "./split/split_%d.txt", file_num++);
            curr_file = fopen(map_filename, "w");
            
            curr_file_size = 0;  // Reset the file size.
        }
        fprintf(curr_file, "%s ", curr_str);
        
        curr_file_size += strlen(curr_str) + 1;
    }
}

key_value* _map(int index) {
    char filename[50];
    sprintf(filename, "./split/split_%d.txt", index);
    FILE *fp_in = fopen(filename, "r");
    char key[500];
    key_value *head, *curr; 
    head = curr = NULL;
    
    while (fscanf(fp_in, "%s", key) > 0) {
        // Set default value for the values.
        int* default_value = malloc(sizeof(int));
        *default_value = 1;
        
        if (head == NULL) {
            head = (key_value*) malloc(sizeof(key_value));;
            curr = head;
        } 
        else{
            curr->next = (key_value*)malloc(sizeof(key_value));;
            curr = curr->next;
        }
        
        // Move the char pointer forward until alphabet character. 
        char* key_copy = malloc(sizeof(key));
        strcpy(key_copy, key);
        
        int i = 0;
        while (i < sizeof(key) && !isalpha(key[i])){
            i++;
            key_copy++;
        }
        
        // Move i from back until alphabet character.
        i = strlen(key_copy) - 1;
        while (i > 0 && !isalpha(key_copy[i])) 
            i--; 
        
        // Add null character at i + 1. 
        key_copy[i+1] = '\0';
        
        curr->key = key_copy;
        curr->value = (void*) default_value;
        curr->next = NULL;
    }
    
    fclose(fp_in);
    return head;
}

key_value* _reduce(char* key, LinkedList* list) {
    key_value* kv = calloc(1, sizeof(kv));
    kv->key = key;
    
    int* result = calloc(1, sizeof(int));
    for (LinkedList* temp_list = list; temp_list != NULL; temp_list = temp_list->next){
        *result = *result + temp_list->value;
    }
    kv->value = (void *)result;
    return kv;
}

int _shuffle(char* key) {
    switch(key[0]) {
        case 97 ... 99:
        case 65 ... 67:
            return 0;
        
        case 100 ... 102:
        case 68 ... 70:
            return 1;
        
        case 103 ... 105:
        case 71 ... 73:
            return 2;

        case 106 ... 108:
        case 74 ... 76:
            return 3;

        case 109 ... 111:
        case 77 ... 79:
            return 4;

        case 112 ... 114:
        case 80 ... 82:
            return 5;
        
        case 115 ... 117:
        case 83 ... 85:
            return 6;
        
        case 118 ... 120:
        case 86 ... 88:
            return 7;

        case 121 ... 123:
        case 89 ... 91:
            return 8;

        default:
            return 9;

    }
        
}

int main(){
    initialize_map_reduce(NUMBER_OF_MAPPERS, NUMBER_OF_REDUCERS, &_split, &_map, &_reduce, &_shuffle);
    run_map_reduce();
    return 0;
}
