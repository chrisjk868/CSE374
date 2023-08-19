all: t9

t9: tnine.o trie.o
	gcc -Wall -std=c11 -g -o t9 tnine.o trie.o

tnine.o: trienode.h
	gcc -Wall -std=c11 -g -c tnine.c

trie.o: trienode.h
	gcc -Wall -std=c11 -g -c trie.c

clean:
	rm -f *.o t9 *~