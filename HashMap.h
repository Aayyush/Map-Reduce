#ifndef HASHMAP_H
#define HASHMAP_H

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

HashMap* initialize_hash_map();
void insert_hash_pair(HashMap*, char*, void*, int);
KeyValue* retrieve_hash_pair(HashMap*, char*);
void destroy_hash_table(HashMap*);

// Calculate the hash value of the given string.
int hash(unsigned char*);

#endif