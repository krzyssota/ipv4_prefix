CC = gcc
CFLAGS = -Wall -Wextra

.PHONY: all clean

all: main

main: main.o utils.o trie.o
	$(CC) -o $@ $^

trie.o: trie.c
	$(CC) $(CFLAGS) -c $<

utils.o: utils.c
	$(CC) $(CFLAGS) -c $<

main.o: main.c utils.h trie.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o main
