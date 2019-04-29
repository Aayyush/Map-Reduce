#include "HashMap.h"
typedef struct key_value key_value;

typedef struct key_value {
    char* key;
    void* value;
    key_value* next;
} key_value;

// char** split(char* file_name);

// char** map(char* file_name);

// void reduce(HashMap* hash_map);

// int shuffle(char* key);

char** (*split) (char*);
char** (*map) (char*);
void (*reduce)(HashMap*);
int (*shuffle) (char*);

void initialize_map_reduce(char** (*s) (char*) , char** (*m) (char*) , void (*r)(HashMap*) , int (*sf) (char*));

void* run_mapper(void* args);

void* run_reducer(void* args);

void run_map_reduce();
