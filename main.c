#include <stdio.h>
#include <stdlib.h>
#include "MapReduce.h"
#include <string.h>
#include <stdbool.h>

#define NUMBER_OF_MAPPERS 10
#define NUMBER_OF_REDUCERS 4

// char** _split(char* fileName, int number_of_mappers) {
//     FILE *input_file;
//     char *word; 
//     int word_count = 0; 
//     FILE *file_handles[number_of_mappers];
//     FILE *input_fh; 
//     FILE *output_file_handle; 
//     int words_written;
    
//     // Declare a pointer to array of strings for fileNames. 
//     char ***list_of_split_files = (char***)malloc(sizeof(char**) * number_of_mappers);
    
//     // Open all the file handles. 
//     for(int i = 0; i < number_of_mappers; ++i){
//         char *address_to_write = (*list_of_split_files) + i * sizeof(char*);
//         printf("File name is : %s\n", get_split_filename_from_mapper_index(i));
//         address_to_write = get_split_filename_from_mapper_index(i);
//         file_handles[i] = fopen(get_split_filename_from_mapper_index(i), "w+");
//     }
    
//     printf("File handles open\n");
    
//     words_written = 0;
//     input_file = fopen(fileName, "r");
//     while(fscanf(input_file, "%s", word) > 1){
//         // Get the file to write to using count%number_of_mappers. 
//         printf("Word: %s\n", word);
//         output_file_handle = file_handles[words_written % number_of_mappers];
//         fprintf(output_file_handle, "%s\n", word);
//     }
    
//     return *list_of_split_files;
// }


void _split(char* filename, int num_splits){
    FILE* fp;
    fp = fopen(filename, "r");

    fseek(fp, 0, SEEK_END); // seek to end of file.
    int size = ftell(fp); // get current file pointer.
    fseek(fp, 0, SEEK_SET); // seek to the beginning of the file.

    int split_at = ceil(size / num_splits);
    int file_num = 0;
    int char_at = 0;
    char last_char;
    char* curr_str;
    FILE* curr_file;

    printf("here");
    while (fscanf(fp, "%s", curr_str) > 1){
        printf("%s", curr_str);
    }
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
    
    char *filename = "input.txt";
    
    _split(filename, NUMBER_OF_MAPPERS);
//     printf("Here\n");
    
    
//     run_mapper(t);
//     // run_reducer(t);
    return 0;
}
