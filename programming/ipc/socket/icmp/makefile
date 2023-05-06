LIBS=-pthread
all:
	gcc ping.c -o ping
	gcc myping.c -o myping
	gcc icmp_checksum.c -o checksum
	gcc myicmpping.c -o icmpping $(LIBS)
