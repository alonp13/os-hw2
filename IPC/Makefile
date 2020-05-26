CC=gcc
FLAGS= -Wall -g

all: ping_pong

ping_pong: ping_pong.c 
	$(CC) $(FLAGS) -o ping_pong ping_pong.c 

.PHONY: clean all

clean:
	rm -f *.o  ping_pong

