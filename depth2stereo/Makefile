CC		= gcc
CFLAG	= -Wall -O
CCSTD	= -I$(INCL) -L$(LIBS)
MAKE	= make
BUILD	= `pwd`/../../build
INCL	= ./build/include/
LIBS	= ./build/lib/
SHELL	= /bin/sh

.SUFFIXES:
.SUFFIXES: .c .o

all:	depth2stereo

depth2stereo:	depth2stereo.c build/lib/libtga.a
		$(CC) $(CFLAG) $(CCSTD) -ltga -o $@ $<

build/lib/libtga.a:
		cd ./lib/libtga && \
		./configure --prefix=$(BUILD) && \
		$(MAKE) && \
		$(MAKE) install

etest:	etest.c q.c q.h
		$(CC) $(CFLAG) -o etest etest.c q.c

stest:	stest.c q.c q.h
		$(CC) $(CFLAG) -o stest stest.c q.c

.PHONY : clean

clean:
		rm -rf ./build
		cd ./lib/libtga && make clean
