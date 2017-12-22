#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "ProcessBurst.hpp"
#include "CPUBurst.hpp"
#include "IOBurst.hpp"
#include "Stats.hpp"

#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <limits>

struct Simulator
{
  double switchCost,
         taskMix,
         currentTime,
         taskFrequency,
         lastJobTime,
         endTime;
  int CPUCount,
      IOCount;

  CPUBurst *CPUQueue;
  IOBurst *IOQueue;
  std::queue<ProcessBurst*> queue;
  std::vector<ProcessBurst*> completed;
  std::vector<double> efficiency;

  std::default_random_engine rng;
  std::bernoulli_distribution boundDistribution;
  std::uniform_int_distribution<int> burstDistribution;

  Simulator();
  Simulator(double switchCost, double taskMix, double taskFrequency, double endTime, int CPUCount, int IOCount);

  void beginSimulation();
  void addTask();
  ProcessBurst* getBurst();
  void pushBurst(ProcessBurst *burst);
  int getNextTask();

  void runCPU();
  void runIO();
  void moveTaskToCPU();
  void clearQueues();

  void calcEfficiency();
  void printStats();
};

#endif
