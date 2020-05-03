CC=g++
CFLAGS = -g -Wall
objects = main.o vector.o objects.o

main:$(objects)
	$(CC) -o main $(objects)

main.o: main.cpp objects.o vector.o
	$(CC) $(CFLAGS) -c main.cpp -o main.o

objects.o: objects.cpp objects.hpp vector.o functions.hpp
	$(CC) $(CFLAGS) -c objects.cpp -o objects.o

vector.o: vector.cpp vector.hpp
	$(CC) $(CFLAGS) -c vector.cpp -o vector.o

.PHONY:clean
clean:
	rm -rf $(objects) main