
TEST_SIZE=$(size)
TEST_NLOOP=$(nloop)

all:
	@echo make test size=1500 nloop=100000
test:
	gcc server.c common.c -o server.out -lm -DTEST_NLOOP=$(TEST_NLOOP) -DTEST_SIZE=$(TEST_SIZE) 
	gcc client.c common.c -o client.out -lm -DTEST_NLOOP=$(TEST_NLOOP) -DTEST_SIZE=$(TEST_SIZE) 
clean:
	rm -f *~ *.out client server
