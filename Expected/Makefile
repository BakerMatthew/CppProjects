CC=g++
CXXFLAGS=-std=c++14 -Wall -Wextra --pedantic
DEBUG=-O3 -g0

all: runfile

runfile: Expected.cpp
	$(CC) $(CXXFLAGS) $(DEBUG) Expected.cpp -o run

clean:
	rm -rf *.o *.tar a.out run

tar:
	tar zcvf MattBaker.tar Expected.cpp Makefile readme
