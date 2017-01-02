CC=g++
INCLUDES=-I/usr/local/include/eigen3
CFLAGS=-O3 -Wall -Wextra -pedantic -DNDEBUG -march=native -mtune=native
LIBS=-fopenmp -DEIGEN_DONT_PARALLELIZE 
TARGET=sigmaclipping

all:
	$(CC) -o $(TARGET) $(CFLAGS) $(INCLUDES) main.cpp

omp:
	$(CC) -o $(TARGET) $(CFLAGS) $(INCLUDES) $(LIBS) main.cpp

clean:
	git clean -f
