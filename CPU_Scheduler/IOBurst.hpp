#ifndef IO_BURST_HPP
#define IO_BURST_HPP

#include "ProcessBurst.hpp"

#include <vector>
#include <queue>

struct IOBurst
{
  int IOCount;
  std::vector<std::vector<ProcessBurst*>> queue;
  std::vector<std::queue<ProcessBurst*>> readyQueue;

  IOBurst(int IOCount);

  int getMinBurst();
  bool notEmpty();
  void lowerIOTime(double time);
  void popReadyQueue();
};

#endif
