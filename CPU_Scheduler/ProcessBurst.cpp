#include "ProcessBurst.hpp"

ProcessBurst::ProcessBurst()
{
  startTime = 0;
  endTime = 0;
  responseTime = 0;
  latency = 0;
  currentTime = 0;
  firstIO = false;

  bursts.push_back(std::numeric_limits<double>::max());
  devices.push_back(0);
}

ProcessBurst::ProcessBurst(double currentTime)
{
  startTime = 0;
  endTime = 0;
  responseTime = 0;
  latency = 0;
  this->currentTime = currentTime;
  firstIO = false;

  bursts.push_back(std::numeric_limits<double>::max());
  devices.push_back(0);
}

ProcessBurst::ProcessBurst(int IOCount, int processCount, double currentTime, bool cpuBound, std::default_random_engine rng)
{
  startTime = currentTime;
  endTime = 0;
  responseTime = 0;
  latency = 0;
  this->currentTime = currentTime;
  firstIO = false;

  double CPULowBound = 0.25,
         IOLowBound = 0.25,
         CPUHighBound,
         IOHighBound;

  if (cpuBound)
  {
    CPUHighBound = 25;
    IOHighBound = 10;
  }
  else
  {
    CPUHighBound = 10;
    IOHighBound = 25;
  }

  CPUTimeDistribution = std::uniform_real_distribution<double>(CPULowBound, CPUHighBound);
  IOTimeDistribution = std::uniform_real_distribution<double>(IOLowBound, IOHighBound);

  //Verify that processBursts start and end with a CPU
  if (processCount % 2 == 0)
  {
    processCount++;
  }
  //Fill bursts and devices with processes
  for (int i = 0; i < processCount; i++)
  {
    if (i % 2 == 0)
    {
      bursts.push_back(CPUTimeDistribution(rng));
    }
    else
    {
      bursts.push_back(IOTimeDistribution(rng));
      IODeviceDistribution = std::uniform_int_distribution<int>(0, IOCount-1);
      devices.push_back(IODeviceDistribution(rng));
    }
  }
}
