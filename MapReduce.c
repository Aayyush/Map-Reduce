#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pthread.h>

#include "MapReduce.h"

// Function signatures for the user-input. 
char** (*split) (char*, int);
key_value* (*map) (int);
key_value* (*reduce)(char*, LinkedList*);
int (*shuffle) (char*);

int num_mappers;
int num_reducers; 

void initialize_map_reduce(int n_mappers, int n_reducers, char** (*s) (char*, int) , key_value* (*m) (int) , key_value* (*r)(char*, LinkedList*) , int (*sf) (char*)){
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
        out_file_location = get_output_file_location(reducer_directory, mapper_index);
        file_handles[i] = fopen(out_file_location, "w");
    }
    
     key_value_list = (*map)(mapper_index); 
    
    int i = 0;
    for (key_value *curr = key_value_list; curr != NULL; curr = curr->next) { 
        reducer_index = (*shuffle)(curr->key);
        fprintf(file_handles[reducer_index], "%s %d\n", curr->key, *(int*)curr->value);
        i++;
    }
    
    for (int i = 0; i < num_reducers; i++){
        fclose(file_handles[i]);
    }
    return NULL;
}

void* run_reducer(void* args){
    int reducer_index = *(int *) args;
    HashMap* map = initialize_hash_map();
    
    
    // Loop through all the keys in it's associated files.
    for (int i = 0; i < 1; i++){  // TODO: Replace 1 with num_mappers.
        
        // Get name and open the file.
        char filename[100];
        sprintf(filename, "./reducer_00/reduce.txt");  // TODO: Change this.
        FILE* temp_reducer_file = fopen(filename, "r");
        
        // Loop through the key-value pairs and store them in hashmap.
        char key_buffer[500];
        int value;
        while(fscanf(temp_reducer_file, "%s %d", key_buffer, &value) > 1){
            
            // Initialize the linked list.
            LinkedList* curr_list = calloc(1, sizeof(LinkedList));
            curr_list->value = value;
            curr_list->next = NULL;
            
            KeyValue* ret_key_val;
            ret_key_val = retrieve_hash_pair(map, key_buffer);
            
            if (ret_key_val == NULL){  // Key isn't in the map. 
                // Make a copy of the key.
                char* temp_key = calloc(1, strlen(key_buffer) +1);
                strcpy(temp_key, key_buffer);
                
                // Insert the linked list into hashmap.
                insert_hash_pair(map, temp_key, curr_list, sizeof(curr_list));
                
            } else {  // Key is in the map.
                
                // Insert the new value into the linked list.
                LinkedList* ret_list = (LinkedList *)ret_key_val->value;
                
                LinkedList* temp_next = ret_list->next;
                ret_list->next = curr_list;
                curr_list->next = temp_next;
            }
        }
        fclose(temp_reducer_file);  // Close the file.
    }
    
    char filename[100];
    sprintf(filename, "./reducer_00/result.txt");
    FILE* fp = fopen(filename, "w");
    
    // Get the keys.
    HashMapIterator* iter = initialize_hash_map_iterator(map);
    KeyValue* curr_kv;
    while ((curr_kv = next_hash_map_item(iter))){
        key_value* ret_kv = (*reduce)(curr_kv->key, (LinkedList *)curr_kv->value);
        fprintf(fp, "%s %d\n", ret_kv->key, *(int *)ret_kv->value);
        free(ret_kv->value);
        free(ret_kv);
    }
    destroy_hash_map_iterator(iter);
    
    return NULL;
}

void run_map_reduce(){
    
    // Split the files.
    (*split)("input.txt", num_mappers);
    
     // Create map threads.
    pthread_t mapper_ids[num_mappers];
    int mapper_index[num_mappers];
    for (int i = 0; i < num_mappers; i++){
        mapper_index[i] = i;
        pthread_create(&mapper_ids[i], NULL, run_mapper, (void *) &mapper_index[i]);
    }
    
    // Join the map threads.
    for (int i = 0; i < num_mappers; i++){
        pthread_join(mapper_ids[i], NULL);
    }
    
//     // Create reduce threads.
//     pthread_t reducer_ids[num_reducers];
//     int reducer_index[num_reducers];
//     for (int i = 0; i < num_reducers; i++){
//         reducer_index[i] = i;
//         pthread_create(&reducer_ids[i], NULL, run_reducer, (void*) &reducer_index[i]);
//     }
    
//     // Join reduce threads.
//     for (int i = 0; i < num_reducers; i++){
//         pthread_join(reducer_ids[i], NULL);
//     }
    
//     // Join the files.
//     return;
}

char* get_reducer_directory_from_index(int index) {
    char* directory_name = (char*)malloc(11);
    sprintf(directory_name, "reducer_0%d", index);
    return directory_name;
}

char* get_output_file_location(char* reducer_directory, int mapper_index) {
    char* directory_name = (char*) malloc(50);
    sprintf(directory_name, "%s/map_%d.txt", reducer_directory, mapper_index);
    return directory_name;
}

char* get_file_name_from_mapper_index(int index) {
    // char* basename = "split_0";
    // char* extension = ".txt";
    char* file_name = (char*)malloc(13);
    sprintf(file_name, "split_0%d.txt", index);
    return file_name;
}

char* get_split_filename_from_mapper_index(int index){
    char* file_name = (char*)malloc(21);
    sprintf(file_name, "./split/split_0%d.txt", index);
    return file_name;
}