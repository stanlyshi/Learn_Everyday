objects := $(patsubst %.c,%.o,$(wildcard *.c))


all: server client 

libstubs.a: server_stub.o client_stub.o 
	ar r libstubs.a server_stub.o client_stub.o

$(objects): %.o: %.c rpc_types.h
	gcc -c $< -o $@

server: libstubs.a myserver.c
	gcc myserver.c libstubs.a -o server
client: libstubs.a myclient.c
	gcc myclient.c libstubs.a -o client


clean:
	rm -rf a.out *.o core *.a server client
