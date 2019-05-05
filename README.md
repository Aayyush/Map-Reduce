# Map-Reduce

A simple implementation of Map Reduce in C using multi-threading. We provide a MapReduce.h library, which the user imports and
passes the map, and reduce functions as specified in the function signature. 

The library includes main.c file which has a mapper function, shuffle function, and a reducer function passed on to the 
MapReduce library, which computes the result and writes it to the results.txt file. 

### Split Function Signature
```
void _split (char* filename, int numberOfSplits) 
filename -> Input Filename "input.txt" by default. 
numberOfSplits -> 10 by default. 
```

### Shuffle Function Signature
```
int _shuffle(char* key) 
key -> Key to be used in indexing the mappers. 
Returns the reducer index to be used for this key. 
```

### Map Function Signature
```
key_value* _map(int index)
index -> Mapper index
Returns a linked_list of key_value. 
```

### Reduce Function Signature
```
key_value* _reduce(char* key, LinkedList* list)
key -> Record Key
list -> Linked list of aggregated records. 
```

## Applications [Word Count]
We provide a input.txt file which contains a long string. The main.c file implements the shuffle, map, and reduce functions, and
the word count in the input file is written to results.txt.

# Contributors
- Aayush Gupta
- Bijesh Subedi
