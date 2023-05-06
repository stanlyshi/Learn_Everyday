
LIBSHMQ=libshmq.so

TEST_P=producer 
TEST_C=comsumer


all: $(LIBSHMQ) 
	gcc -pthread -lrt test_producer.c -o $(TEST_P) ./$(LIBSHMQ)
	gcc -pthread -lrt test_comsumer.c -o $(TEST_C) ./$(LIBSHMQ)


$(LIBSHMQ):
	gcc -shared -fpic shmq.c -pthread -lrt -o $(LIBSHMQ)
	
clean:
	rm -f $(TEST_P) $(TEST_C) $(LIBSHMQ)
