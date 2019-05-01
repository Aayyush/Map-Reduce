#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "MapReduce.h"

// Function signatures for the user-input. 
char** (*split) (char*);
key_value* (*map) (char*);
void (*reduce)(HashMap*);
int (*shuffle) (char*);

int num_mappers;
int num_reducers; 

void initialize_map_reduce(int n_mappers, int n_reducers, char** (*s) (char*) , key_value* (*m) (char*) , void (*r)(HashMap*) , int (*sf) (char*)){
    num_mappers = n_mappers;
    num_reducers = n_reducers;
    split = s;
    map = m;
    reduce = r;
    shuffle = sf;
}

void* run_mapper(void* args){
    char* reducer_directory;
    char* out_file_location; 
    int reducer_index;
    struct stat sb;
    int mapper_index = *(int*)args;
    key_value* key_value_list; 

    // File handles for each reducer to write to. 
    FILE *file_handles[num_reducers];
    
    // Split file to read from, used mapper_index. 
    char* input_filename = get_file_name_from_mapper_index(mapper_index);
    
    // Make directory for all the reducers. 
    // Open file handles for each reducer. 
    for (int i = 0; i < num_reducers; ++i){
        reducer_directory = get_reducer_directory_from_index(i);
        
        // Make directory if not exist. 
        if (!(stat(reducer_directory, &sb) == 0 && S_ISDIR(sb.st_mode))){
            mkdir(reducer_directory, 0777);
        }
        
        // Output file location is ./reducer_directory/mapper_file
        out_file_location = get_output_file_location(reducer_directory, i);
        file_handles[i] = fopen(out_file_location, "w");
    }
    
    // Run user-defined map function.
    // Returns a linked list of key value pairs. 
    key_value_list = (*map)(input_filename);
    
    int i = 0;
    while(key_value_list != NULL) {
        // User-defined shuffle function to get reducer index to write to. 
        reducer_index = (*shuffle)(key_value_list->key);
        fprintf(file_handles[reducer_index], "(%s, %d)\n", key_value_list->key, *((int *)key_value_list->value));
        
        // Free each node as you go. 
        key_value *temp = key_value_list;
        key_value_list = key_value_list->next;
        free(temp);
        i++;
    }
    
    // Close all file_handles
    for(int i = 0; i < num_reducers; ++i) {
        fclose(file_handles[i]);
    }
    
    free(reducer_directory);
    free(out_file_location);
}

void* run_reducer(void* args){
    
}

void run_map_reduce(){
    // Check all the functions are properly configured. 
    // Split files based on number of mappers. 
    // Create threads for each split, call run_mapper with index of the mapper. 
    // Wait for all the mappers to finish execution. 
    // Create thread for each reducer, call run_reducer and pass index of the reducer. 
    // Wait for all the reducers 
    // Merge all output files. 
}

char* get_reducer_directory_from_index(int index) {
    char* directory_name = (char*)malloc(11);
    sprintf(directory_name, "reducer_0%d", index);
    return directory_name;
}

char* get_output_file_location(char* reducer_directory, int index) {
    char* directory_name = (char*)malloc(strlen(reducer_directory) + 14);
    sprintf(directory_name, "%s/split_0%d.txt", reducer_directory, index);
    return directory_name;
}

char* get_file_name_from_mapper_index(int index) {
    // char* basename = "split_0";
    // char* extension = ".txt";
    char* file_name = (char*)malloc(13);
    sprintf(file_name, "split_0%d.txt", index);
    return file_name;
}