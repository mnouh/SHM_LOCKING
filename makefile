all:main

main: main.o
	gcc  main.o -g -o main -lrt
main.o: main.c
	gcc -c main.c -g -o main.o -lrt
clean:
	rm -f *.o main
