#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "HashMap.h"

HashMap* initialize_hash_map(){
    HashMap* new_map = calloc(1, sizeof(HashMap));
    for (int i = 0; i < HASHTABLE_SIZE; i++){
        new_map->hash_map[i] = NULL;
    }
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
    
    // Make a copy of value. Making a copy of key isn't required because
    // c string are stored in heap by convention.
    void* value_copy = malloc(sizeof(value));
    memcpy(value_copy, value, size);
    value = value_copy;
    
    // Check if the key already exists going through the linked list.
    KeyValue* curr_kv = map->hash_map[hash_index];    
    while (curr_kv != NULL){
        // Update the value with new value if the key matches an old entry.
        if (strcmp(curr_kv->key, key) == 0){
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
        if (strcmp(curr_kv->key, key) == 0){
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

HashMapIterator* initialize_hash_map_iterator(HashMap* map){
    HashMapIterator* new_iter = calloc(1, sizeof(HashMapIterator));
    new_iter->map = map;
    new_iter->hash_index = HASHTABLE_SIZE - 1;
    new_iter->curr_kv = NULL;
    // Get the first valid key_value pair.
    for (int i = 0; i < HASHTABLE_SIZE; i++){
        KeyValue* temp_kv = new_iter->map->hash_map[i];
        if (temp_kv) {
            new_iter->curr_kv = temp_kv;
            new_iter->hash_index = i;
            break;
        }
    }
    return new_iter;
}

KeyValue* next_hash_map_item(HashMapIterator* iter){
    // Check if the iterator has already reached the end.
    if (iter->hash_index == HASHTABLE_SIZE - 1 && iter->curr_kv == NULL){
        return NULL;
    }
    
    KeyValue *return_kv = iter->curr_kv;  // Item to be returned.
    KeyValue *temp_kv = iter->curr_kv->next;
    // Assign the next item on the map.
    for (int i = iter->hash_index; i < HASHTABLE_SIZE; i++){
        if (i != iter->hash_index){
            temp_kv = iter->map->hash_map[i];
        }
        if (temp_kv){
            iter->curr_kv = temp_kv;
            iter->hash_index = i;
            return return_kv;
        }
    }
    iter->curr_kv = NULL;
    return NULL;
}



