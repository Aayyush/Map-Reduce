#include "HashMap.h"

typedef struct key_value {
    char* key;
    void* value;
    key_value* next;
} key_value;


void map(char* fileName);

void reduce();

void split();

void shuffle();


void init()