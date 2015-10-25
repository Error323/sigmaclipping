CC=clang++-3.6
CFLAGS= -O2 -Wall -Wextra -std=c++11 -march=native -I/usr/local/include/eigen3

all:
	$(CC) -o sigmaclip $(CFLAGS) main.cpp

clean:
	git clean -f
