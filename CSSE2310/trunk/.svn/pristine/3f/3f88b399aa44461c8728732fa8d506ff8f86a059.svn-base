CC = gcc
CFLAGS = -Wall -pedantic -std=gnu99
.PHONY: all

all: 2310hub 2310alice 2310bob

2310hub: 2310hub.c
	$(CC) $(CFLAGS) 2310hub.c -o 2310hub

2310alice: 2310alice.c 2310sharedPlayer.c
	$(CC) $(CFLAGS) 2310alice.c 2310sharedPlayer.c -o 2310alice

2310bob: 2310bob.c 2310sharedPlayer.c
	$(CC) $(CFLAGS) 2310bob.c 2310sharedPlayer.c -o 2310bob



clean:
	rm 2310hub 2310alice 2310bob



