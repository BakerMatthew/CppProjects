#ifndef CPU_BURST_HPP
#define CPU_BURST_HPP

#include "ProcessBurst.hpp"

#include <vector>

struct CPUBurst
{
  int CPUCount;
  std::vector<ProcessBurst*> queue;

  CPUBurst(int CPUCount);

  int getMinBurst();
  bool notEmpty();
  void lowerCPUTime(double time);
};

#endif
