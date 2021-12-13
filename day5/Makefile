CC = g++
CFLAGS = -std=c++2a -Wpedantic -Wall -Wextra -Wconversion -O3 -g

EXEC = ./prog
SRC = ./main.cpp

all: prog

prog: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC)

run: prog
	$(EXEC)

clean:
	rm -rf $(EXEC)

.PHONY: all clean run


