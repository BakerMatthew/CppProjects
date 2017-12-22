#ifndef PROCESS_BURST_HPP
#define PROCESS_BURST_HPP

#include <vector>
#include <random> //std::uniform_real_distribution and std::poisson_distribution
#include <limits> //std::numeric_limits<double>::max()

struct ProcessBurst
{
  double startTime,
         endTime,
         responseTime,
         latency,
         currentTime;
  bool firstIO;

  std::vector<double> bursts;
  std::vector<int> devices;

  std::uniform_real_distribution<double>  CPUTimeDistribution,
                                          IOTimeDistribution;
  std::uniform_int_distribution<int> IODeviceDistribution;

  ProcessBurst();
  ProcessBurst(double currentTime);
  ProcessBurst(int IOCount, int processCount, double currentTime, bool cpuBound, std::default_random_engine generator);
};

#endif
