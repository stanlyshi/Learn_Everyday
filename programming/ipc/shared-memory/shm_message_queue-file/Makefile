RADE1_BIN=reader
WRITE_BIN=writer
READ_SRC1=shm_queue.c test_reader.c
WRITE_SRC=shm_queue.c test_writer.c
FLAGS=-g -Wall
INCLUDE=-I./
CC=gcc

.PHONY:all
all:$(RADE1_BIN) $(RADE2_BIN)  $(WRITE_BIN)
$(RADE1_BIN):$(READ_SRC1)	
	$(CC) $^ -o $@ $(FLAGS) $(INCLUDE)
$(WRITE_BIN):$(WRITE_SRC)	
	$(CC) $^ -o $@ $(FLAGS) $(INCLUDE)
.PHONY:clean
clean:
	rm -rf  $(RADE1_BIN) $(WRITE_BIN)