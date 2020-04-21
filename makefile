vpath %.c src
vpath %.o bin
vpath remesg bin

CC = gcc
CFLAGS = -c -Wall -Wextra -pedantic-errors -std=c89
OBJS = main.o

.PHONY: all clean
all: remesg

clean:
	rm bin/*

bin/remesg: $(OBJS)
	$(CC) $^ -o $@

bin/main.o: main.c
	$(CC) $(CFLAGS) $< -o $@
