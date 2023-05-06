#编译

LIBS := -pthread -lrt

SOURCES := main.c utils.c

CC=gcc

TARGET = main

all: 
	$(CC) -o $(TARGET) $(SOURCES) $(LIBS)

clean:
	rm -rf $(TARGET)
