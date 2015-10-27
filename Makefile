CC=g++
CFLAGS= -O3 -Wall -Wextra -pedantic -fopenmp -DEIGEN_DONT_PARALLELIZE -DNDEBUG -mavx -I/usr/local/include/eigen3

all:
	$(CC) -o sigmaclip $(CFLAGS) main.cpp

clean:
	git clean -f
