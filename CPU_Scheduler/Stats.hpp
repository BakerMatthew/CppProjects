#ifndef STATS_HPP
#define STATS_HPP

#include "ProcessBurst.hpp"

#include <vector>
#include <queue>
#include <limits> //std::numeric_limits<double>::max()
#include <cmath>

struct Stats
{
private:
  double endTime;
  std::queue<ProcessBurst*> queue;
  std::vector<ProcessBurst*> completed;
  std::vector<double> efficiency;

public:
  int resonseTimeSize = 0;
  double minLatency = std::numeric_limits<double>::max(),
         maxLatency = 0,
         avgLatency = 0,
         latencyDeviation = 0,
         throughput = 0,
         avgThroughput = 0,
         minResponseTime = std::numeric_limits<double>::max(),
         maxResponseTime = 0,
         avgResponseTime = 0,
         responseTimeDeviation = 0,
         minEfficiency = std::numeric_limits<double>::max(),
         maxEfficiency = 0,
         avgEfficiency = 0,
         efficiencyDeviation = 0;

  Stats(double endTime, std::queue<ProcessBurst*> queue, std::vector<ProcessBurst*> completed, std::vector<double> efficiency);

  void computeStats();
};

#endif
