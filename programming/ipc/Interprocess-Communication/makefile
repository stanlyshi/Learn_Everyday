#
# This is an example Makefile for a countwords program.  This
# program uses both the scanner module and a counter module.
# Typing 'make' or 'make count' will create the executable file.
#

# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
#
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#
# for C++ define  CC = g++
CC = gcc
CFLAGS  = -g -Wall

# typing 'make' will invoke the first target entry in the file 
# (in this case the default target entry)
# you can name this target entry anything, but "default" or "all"
# are the most commonly used names by convention
#
all: client_main server_main

# To create the executable file count we need the object files
# countwords.o, counter.o, and scanner.o:
#

client_main: client.o queue.o
	$(CC) $(CFLAGS) -o client_main client.o queue.o 	

client.o: client.c client.h
	$(CC) $(CFLAGS) -c client.c

server_main: server-msgq.o queue.o 
	$(CC) $(CFLAGS) -o server_main server-msgq.o queue.o 

# To create the object file counter.o, we need the source files
# counter.c and counter.h:
#
server-msgq.o: server-msgq.c server.h
	$(CC) $(CFLAGS) -c server-msgq.c 
# To create the object file countwords.o, we need the source
# files countwords.c, scanner.h, and counter.h:
#

# To create the object file scanner.o, we need the source files
# scanner.c and scanner.h:
#
queue.o:  queue.c queue.h 
	$(CC) $(CFLAGS) -c queue.c

# To create the object file counter.o, we need the source files
# counter.c and counter.h:
#
#server.o: server.c server.h
#	$(CC) $(CFLAGS) -c server.c

#semaphore.o:  Semaphore.c Semaphore.h 
#	$(CC) $(CFLAGS) -c Semaphore.c

# To create the object file scanner.o, we need the source files
# scanner.c and scanner.h:
#
#queue.o:  queue.c queue.h 
#	$(CC) $(CFLAGS) -c queue.c		

# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean: 
	$(RM) count *.o *~
