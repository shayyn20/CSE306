CC=g++-9
CFLAGS = -g -Wall -I/usr/local/include -L/usr/local/lib -lomp -fopenmp
objects = main.o vector.o objects.o

main:$(objects)
	$(CC) -I/usr/local/include -L/usr/local/lib -lomp -fopenmp -o main $(objects)

main.o: main.cpp objects.o vector.o
	$(CC) $(CFLAGS) -c main.cpp -o main.o

objects.o: objects.cpp objects.hpp vector.o
	$(CC) $(CFLAGS) -c objects.cpp -o objects.o

vector.o: vector.cpp vector.hpp
	$(CC) $(CFLAGS) -c vector.cpp -o vector.o

.PHONY:clean
clean:
	rm -rf $(objects) main