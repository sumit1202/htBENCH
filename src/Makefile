# -*- Makefile -*-

all: httpGetRequest

httpGetRequest: httpGetRequest.o
	gcc httpGetRequest.o -o httpGetRequest

httpGetRequest.o: httpGetRequest.c
	gcc -c httpGetRequest.c

clean:
	rm -rf *o httpGetRequest