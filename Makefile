#Makefile for Minichess Project

#set up flags
CFLAGS = -std=c++11 -g -Wall -O3
CC = g++

Minichess: main.cpp board.cpp
	$(CC) main.cpp board.cpp player.cpp netops.cpp $(CFLAGS) -I/.

clean:
	rm a.out
