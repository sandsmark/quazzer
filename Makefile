CC=gcc
CFLAGS+=-std=c99 -Wall -pedantic -O2 -g
LDFLAGS+=

ALL = quazzer
all: $(ALL)

quazzer: main.o
	$(CC) $(LDFLAGS) -o $@ $^

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $<

