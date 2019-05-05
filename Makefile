CC = gcc
CFLAGS = -g -pthread

run: main.o MapReduce.o HashMap.o
	$(CC) $(CFLAGS) -o run main.o MapReduce.o HashMap.o
	
main.o: main.c MapReduce.h HashMap.h
	$(CC) $(CFLAGS) -c main.c
	
MapReduce.o: MapReduce.c HashMap.h MapReduce.h
	$(CC) $(CFLAGS) -c MapReduce.c
	
HashMap.o: HashMap.c HashMap.h
	$(CC) $(CFLAGS) -c HashMap.c

clean:
	$(RM) run *.o *~ *.h.gch *.out
	$(RM) -rf reducer_*
	$(RM) -f split/* 