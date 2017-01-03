CC=g++
INCLUDES=-I/usr/local/include/eigen3
CFLAGS=-std=c++14 -O3 -Wall -Wextra -pedantic -DNDEBUG -march=native -mtune=native
TARGET=sigmaclipping

all:
	$(CC) -o $(TARGET) $(CFLAGS) $(INCLUDES) main.cpp

clean:
	git clean -f
