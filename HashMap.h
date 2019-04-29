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
    KeyValue hash_map[HASHTABLE_SIZE];
} HashMap;

#endif