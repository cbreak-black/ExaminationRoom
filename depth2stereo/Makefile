CC		= gcc
CFLAG	= -Wall -O
MAKE	= make
BUILD	= `pwd`/../../build
SHELL	= /bin/sh

.SUFFIXES:
.SUFFIXES: .c .o

all:	depth2stereo

build:	libtga

depth2stereo:
		

libtga:
		cd ./lib/libtga && \
		./configure --prefix=${BUILD} && \
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
