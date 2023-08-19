# Makefile for mem memory system, CSE374 22WI


CC = gcc
CARGS = -Wall -std=c11

all: bench

# basic build
bench: bench.o getmem.o freemem.o mem_utils.o
	$(CC) $(CARGS) -o bench $^

# object files
# fill in your individual object targets here

bench.o: bench.c mem.h mem_impl.h mem_utils.c
	$(CC) $(CARGS) -c bench.c

getmem.o: getmem.c mem.h
	$(CC) $(CARGS) -c getmem.c

freemem.o: freemem.c mem.h
	$(CC) $(CARGS) -c freemem.c

mem_utils.o: mem_utils.c mem.h
	$(CC) $(CARGS) -c mem_utils.c

# You can use these examples for other types of builds
# make sure you know what they do.
noassert: CARGS += -D NDEBUG
noassert: bench

debug: CARGS += -g
debug: bench

test: debug
	./bench 10 50

clean:
	rm -f bench *.o *~ 
