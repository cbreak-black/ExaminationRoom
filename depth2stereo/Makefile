CC		= gcc
CFLAG	= -Wall -O0 -g
CCSTD	= -I$(INCL) -L$(LIBS)
MAKE	= make
BUILD	= `pwd`/../../build
INCL	= .
LIBS	= .
SHELL	= /bin/sh

.SUFFIXES:
.SUFFIXES: .c .o

all:	depth2stereo

depth2stereo:	depth2stereo.c 
		$(CC) $(CFLAG) $(CCSTD) -ltga -o $@ $<

.PHONY : clean

clean:
	rm -f depth2stereo
