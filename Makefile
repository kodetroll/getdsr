#############################################
# 
# Simple makefile for getdsr
#
#IDIR =../include
CC=gcc
CFLAGS=-I.
DEPS = 
OBJ = getdsr.o  
LDFLAGS=-lezV24

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: getdsr
getdsr: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm -f *.o *~ core  

cleanall:
	rm -f getdsr *.o *~ core  

install:
	install getdsr /usr/local/sbin
