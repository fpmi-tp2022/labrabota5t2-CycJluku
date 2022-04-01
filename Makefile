# Makefile for lab 5

all: bin/program

bin/program: obj/main.o obj/server.o
	gcc -o bin/program  obj/main.o obj/server.o -l sqlite3

obj/main.o: src/main.c
	gcc -o obj/main.o -c src/main.c -l sqlite3

obj/server.o: src/server.c
	gcc -o obj/server.o -c src/server.c -l sqlite3

clean:
	rm -rf obj/*.o bin/program
