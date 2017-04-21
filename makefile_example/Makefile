# Implementation of Makefile

# target
TARGET = main

# compiler
CC = gcc

# flags
CFLAGS = -Wall -g
CFLAGS_INCLUDE= -I.

# source code
SRCS = hello_fun.c main.c
OBJS = $(SRCS: .c=.o)

# header
DEPS = hello_fun.h

# object
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(CFLAGS_INCLUDE)


$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(CFLAGS_INCLUDE)


# clean
.PHONY: clean
clean:
	$(RM) $(EXEC) *.o $(TARGET)
