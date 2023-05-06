CFLAGS = -g -O2 -Wall -fPIC -Ilibwuya -std=c99
LDFLAGS = -Llibwuya

libleak.so: libleak.o
	CFLAGS='-fPIC' make -C libwuya
	$(CC) -shared -o $@ $^ $(LDFLAGS) -lwuya -lpthread -ldl -lbacktrace 

clean:
	rm -f libleak.so *.o /tmp/libleak.*
	make -C libwuya clean
