
CC = gcc
FLAGS = -std=c99 -g

draw2:draw.o parser.o util.o
	$(CC) $(FLAGS) -o draw2 draw.o parser.o util.o -lm
draw.o:draw.c
	$(CC) $(FLAGS) -c draw.c -o draw.o
parser.o:parser.c
	$(CC) $(FLAGS) -c parser.c -o parser.o
util.o:util.c
	$(CC) $(FLAGS) -c util.c -o util.o

.PHONY:clean
clean:
	-rm *.o draw2
