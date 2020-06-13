CC=g++-9
CFLAGS = -g -Wall -I/usr/local/include -L/usr/local/lib -lomp -fopenmp

main: main.cpp
	$(CC) $(CFLAGS) main.cpp -o main 

.PHONY:clean
clean:
	rm -rf main