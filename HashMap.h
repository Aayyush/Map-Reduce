#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>

#define HASHTABLE_SIZE 50


typedef struct KeyValue KeyValue;

typedef struct KeyValue {
    char* key;
    void* value;
    KeyValue* next;
} KeyValue;

typedef struct HashMap{
    KeyValue* hash_map[HASHTABLE_SIZE];
} HashMap;

typedef struct HashMapIterator {
    HashMap* map;
    KeyValue* curr_kv;  // Current KeyValue pair.
    int hash_index;  // Current hash index.
} HashMapIterator;

HashMap* initialize_hash_map();
void insert_hash_pair(HashMap*, char*, void*, int);
KeyValue* retrieve_hash_pair(HashMap*, char*);
void destroy_hash_table(HashMap*);

HashMapIterator* initialize_hash_map_iterator(HashMap*);
KeyValue* next_hash_map_item(HashMapIterator*);
void destroy_hash_map_iterator(HashMapIterator*);

// Calculate the hash value of the given string.
int hash(unsigned char*);

#endif