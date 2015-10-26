CC=clang++-3.6
CFLAGS= -O3 -Wall -Wextra -pedantic -DNDEBUG -march=native -I/usr/local/include/eigen3

all:
	$(CC) -o sigmaclip $(CFLAGS) main.cpp

clean:
	git clean -f
