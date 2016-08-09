CC=g++
INCLUDES=-I/net/home/fhuizing/soft/release/include/eigen3
CFLAGS=-O3 -Wall -Wextra -pedantic -DNDEBUG -mavx
LIBS=-fopenmp -DEIGEN_DONT_PARALLELIZE 
TARGET=sigmaclipping

all:
	$(CC) -o $(TARGET) $(CFLAGS) $(INCLUDES) main.cpp

omp:
	$(CC) -o $(TARGET) $(CFLAGS) $(INCLUDES) $(LIBS) main.cpp

clean:
	git clean -f
