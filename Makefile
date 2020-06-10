P=i2p
OBJECTS=*.c
CFLAGS=-Wall -o $(P)
CC=gcc

$(P): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS)
