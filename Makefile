# build a program from 2 files and one shared header
CC = gcc
CFLAGS = -g -Wall

LIBRARIES = `pkg-config --cflags --libs lua-5.3`

program:
	$(CC) $(CFLAGS) embeddingLua.c $(LIBRARIES)
