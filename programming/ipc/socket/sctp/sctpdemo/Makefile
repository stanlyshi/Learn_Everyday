# make

CC := gcc
CFLAGS := -Wall

all: client.out server.out

client.out: client.o
	$(CC) $(CFLAGS) -o $@ client.c -L/usr/local/lib -lsctp

server.out: server.o
	$(CC) $(CFLAGS) -o $@ server.c -L/usr/local/lib -lsctp

clean:
	rm -f client.out server.out *.o 

