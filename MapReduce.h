#ifndef MAPREDUCE_H
#define MAPREDUCE_H

#include "HashMap.h"

typedef struct key_value key_value;

typedef struct key_value {
    char* key;
    void* value;
    key_value* next;
} key_value;

typedef struct LinkedList LinkedList;

typedef struct LinkedList {
    int value;
    LinkedList* next;
} LinkedList;

extern char** (*split) (char*);
extern key_value* (*map) (char*);
extern key_value* (*reduce)(char*, LinkedList*);
extern int (*shuffle) (char*);
extern int num_mappers; 
extern int num_reducers; 

void initialize_map_reduce(int n_mappers, int n_reducers, char** (*s) (char*) , key_value* (*m) (char*) , void (*r)(HashMap*) , int (*sf) (char*));

void* run_mapper(void* args);

void* run_reducer(void* args);

void run_map_reduce();

char* get_file_name_from_mapper_index(int index);

char* get_reducer_directory_from_index(int index);

char* get_output_file_location(char* reducer_directory, int index);

#endif