CC=g++
INCLUDES=-I/usr/local/include/eigen3
CFLAGS=-O3 -Wall -Wextra -pedantic -DNDEBUG -mavx
LIBS=-fopenmp -DEIGEN_DONT_PARALLELIZE 

all:
	$(CC) -o sigmaclip $(CFLAGS) $(INCLUDES) main.cpp

omp:
	$(CC) -o sigmaclip $(CFLAGS) $(INCLUDES) $(LIBS) main.cpp

clean:
	git clean -f
