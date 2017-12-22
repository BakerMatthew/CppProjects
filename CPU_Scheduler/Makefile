CC=g++
CXXFLAGS=-std=c++14 -Wall -Werror -Wextra --pedantic
DEBUG=-O3 -g0

all: runfile

runfile: ProcessBurst.cpp ProcessBurst.hpp SimulatorMenu.cpp Simulator.cpp Simulator.hpp Stats.cpp Stats.hpp CPUBurst.cpp CPUBurst.hpp IOBurst.cpp IOBurst.hpp
	$(CC) $(CXXFLAGS) $(DEBUG) ProcessBurst.cpp ProcessBurst.hpp SimulatorMenu.cpp Simulator.cpp Simulator.hpp Stats.cpp Stats.hpp  CPUBurst.cpp CPUBurst.hpp IOBurst.cpp IOBurst.hpp -o run

clean:
	rm -rf *.o *.tar a.out run

tar:
	tar zcvf MattBaker.tar SimulatorMenu.cpp Simulator.cpp Simulator.hpp Stats.cpp Stats.hpp ProcessBurst.cpp ProcessBurst.hpp CPUBurst.cpp CPUBurst.hpp IOBurst.cpp IOBurst.hpp readme Makefile 

