# Simple Makefile for siphash

CC= gcc
CFLAGS= -fPIC -Wall
CPPFLAGS= -I.
DEPS= siphash.h
OBJ= siphash.o
DLEXT = so

RM= rm -f

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS)

siphash: siphash.$(DLEXT)
siphash.$(DLEXT): $(OBJ)
	$(CC) -shared -o $@ $^

test: $(OBJ)		   
	$(CC) -o $@ $< test.c

clean:
	@-$(RM) *.o test
	@-$(RM) *.$(DLEXT)

.PHONY: siphash clean
