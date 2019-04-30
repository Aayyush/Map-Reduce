#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HashMap.h"

HashMap* initialize_hash_map(){
    HashMap* new_map = calloc(1, sizeof(HashMap));
    return new_map;
}

int hash(unsigned char* str){
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)){
        hash = ((hash << 5) + hash) + c;
    }
    return hash % HASHTABLE_SIZE;
}

void insert_hash_pair(HashMap* map, char* key, void* value, int size){
    int hash_index = hash((unsigned char*) key);  // Get the hash value.

    void* value_copy = malloc(sizeof(value));
    memcpy(value_copy, value, size);
    value = value_copy;
    
    // Check if the key already exists going through the linked list.
    KeyValue* curr_kv = map->hash_map[hash_index];    
    while (curr_kv != NULL){
        // Update the value with new value if the key matches an old entry.
        if (curr_kv->key == key){
            curr_kv->value = value;
            return;  // End here.
        }
        curr_kv = curr_kv->next;
    }
    
    // Initialize a new KeyValue pair to insert.
    KeyValue* new_kv = calloc(1, sizeof(KeyValue));
    new_kv->key = key;
    new_kv->value = value;
    new_kv->next = map->hash_map[hash_index];
    
    // Insert the new pair at the top of the map.
    map->hash_map[hash_index] = new_kv;
}

KeyValue* retrieve_hash_pair(HashMap* map, char* key){
    KeyValue* result = NULL;
    
    // Get the index at which the key was inserted.
    int hash_index = hash((unsigned char*) key);
    KeyValue* curr_kv = map->hash_map[hash_index];

    while (curr_kv != NULL){
        if (curr_kv->key == key){
            result = curr_kv;
            break;
        }
        curr_kv = curr_kv->next;
    }
    return result;
}

void destroy_hash_table(HashMap* map){
    // Loop throught all the indexes.
    for (int i = 0; i < HASHTABLE_SIZE; i++){
        KeyValue* curr_kv = map->hash_map[i];
        while (curr_kv){
            KeyValue* temp = curr_kv;
            curr_kv = curr_kv->next;
            free(temp->value);
            free(temp); 
        }
    }
    free(map);
}



