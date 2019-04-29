#include <stdio.h>
#include "MapReduce.h"


void initialize_map_reduce(char** (*s) (char*) , char** (*m) (char*) , void (*r)(HashMap*) , int (*sf) (char*)){
    split = s;
    map = m;
    reduce = r;
    shuffle = sf;
}

void* run_mapper(void* args){
    
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

int main(){
    return 0;
}