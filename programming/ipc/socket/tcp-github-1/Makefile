LIB=-I . tcp_common.c

ALL:
	gcc client.c -o client ${LIB}
	gcc server.c -o server ${LIB}
	gcc server_ite.c -o server_ite ${LIB}
	gcc server_concurrent.c -o server_con ${LIB}

clean:
	rm client server server_ite server_con

