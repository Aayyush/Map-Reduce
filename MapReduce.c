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
    // Cast args to index. 
    // Get filename from index. 
    // Call map with the filename and get linked list of key and values. 
    // Iterate over each node, get the key, use shuffle to get the reducer index. 
    // Get filename from the reducer index. 
    // Make a directory for the reducer if it does not exist. 
    // If exists, write to the reducer directory. 
    
    int mapper_index = *(int*)args;
    
    FILE *file_handles[num_reducers];
    char* reducer_directory;
    char* out_file_location; 
    int reducer_index;
    struct stat sb;
    key_value* key_value_list; 
    char* input_filename = get_file_name_from_mapper_index(mapper_index);
    
    // Make directory for all the reducers. 
    for (int i = 0; i < num_reducers; ++i){
        reducer_directory = get_reducer_directory_from_index(i);
        if (!(stat(reducer_directory, &sb) == 0 && S_ISDIR(sb.st_mode))){
            mkdir(reducer_directory, 0777);
        }
        out_file_location = get_output_file_location(reducer_directory, i);
        file_handles[i] = fopen(out_file_location, "w");
    }
    
);
    int i = 0;
    for (key_value *curr = key_value_list; curr != NULL; curr = curr->next) { 
        reducer_index = (*shuffle)(curr->key);
        printf("Reducer index: %d\n", reducer_index);
        fprintf(file_handles[reducer_index], "(%s, %s)\n", curr->key, (char *)curr->value);
        i++;
    }
    return;
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