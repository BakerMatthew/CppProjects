#include "Simulator.hpp"

Simulator::Simulator()
{
  switchCost = 0.35;
  taskMix = 0.65;
  currentTime = 0;
  taskFrequency = 10;
  lastJobTime = 0;
  endTime = 10000;
  CPUCount = 7;
  IOCount = 12;

  CPUQueue = new CPUBurst(CPUCount);
  IOQueue = new IOBurst(IOCount);

  boundDistribution = std::bernoulli_distribution(taskMix);
  burstDistribution = std::uniform_int_distribution<int>(4, 16);
}

Simulator::Simulator(double switchCost, double taskMix, double taskFrequency, double endTime, int CPUCount, int IOCount)
{
  this->switchCost = switchCost;
  this->taskMix = taskMix;
  currentTime = 0;
  this->taskFrequency = taskFrequency;
  lastJobTime = 0;
  this->endTime = endTime;
  this->CPUCount = CPUCount;
  this->IOCount = IOCount;

  CPUQueue = new CPUBurst(CPUCount);
  IOQueue = new IOBurst(IOCount);

  boundDistribution = std::bernoulli_distribution(taskMix);
  burstDistribution = std::uniform_int_distribution<int>(4, 16);
}

void Simulator::beginSimulation()
{
  addTask();
  currentTime += taskFrequency;

  while ((int)CPUQueue->queue.size() < CPUCount)
  {
    if (queue.empty())
    {
      break;
    }
    CPUQueue->queue.push_back(queue.front());
    queue.pop();
  }

  int tasksMade = 1;
  while (currentTime < endTime)
  {
    int newTaskCount = (int)(currentTime / taskFrequency) - tasksMade;
    while (newTaskCount > 0)
    {
      addTask();
      lastJobTime -= taskFrequency;
      tasksMade++;
      newTaskCount--;
    }

    switch (getNextTask())
    {
      case -1:
        addTask();
        currentTime += taskFrequency;
        lastJobTime += taskFrequency;
        break;
      case 0:
        runCPU();
        break;
      case 1:
        runIO();
        break;
    }
    moveTaskToCPU();
    IOQueue->popReadyQueue();
    calcEfficiency();
  }
  clearQueues();
  printStats();
}

void Simulator::addTask()
{
  pushBurst(new ProcessBurst(IOCount, burstDistribution(rng), currentTime, boundDistribution(rng), rng));
}

ProcessBurst* Simulator::getBurst()
{
  ProcessBurst *burst = queue.front();
  queue.pop();
  return burst;
}

void Simulator::pushBurst(ProcessBurst *burst)
{
  queue.push(burst);
}

int Simulator::getNextTask()
{
  double minCPUTime = std::numeric_limits<double>::max();
  double minIOTime = std::numeric_limits<double>::max();

  if (CPUQueue->notEmpty())
  {
    minCPUTime = CPUQueue->queue.at(CPUQueue->getMinBurst())->bursts.at(0);
  }
  if (IOQueue->notEmpty())
  {
    minIOTime = IOQueue->queue.at(IOQueue->getMinBurst()).at(0)->bursts.at(0);
  }

  if (CPUQueue->queue.empty())
  {
    bool bothEmpty = true;
    for (int i = 0; i < IOCount; i++)
    {
      if (!IOQueue->queue.at(i).empty())
      {
        bothEmpty = false;
      }
    }
    if (bothEmpty)
    {
      return -1;
    }
  }

  //CPU
  if (minCPUTime <= minIOTime)
  {
    return 0;
  }
  //IO
  else if (minIOTime < minCPUTime)
  {
    return 1;
  }
  //ERROR
  else
  {
    return -1;
  }
}

void Simulator::runCPU()
{
  moveTaskToCPU();

  //Get smallest burst
  int minCPU = CPUQueue->getMinBurst();
  ProcessBurst *minBurst = CPUQueue->queue.at(minCPU);
  CPUQueue->queue.erase(CPUQueue->queue.begin()+minCPU);

  //Get time
  double minTime = minBurst->bursts.at(0);
  minBurst->bursts.erase(minBurst->bursts.begin());

  //Lower time of tasks
  CPUQueue->lowerCPUTime(minTime);
  IOQueue->lowerIOTime(minTime);

  //Move to queue
  if (minBurst->bursts.empty())
  {
    completed.push_back(minBurst);
  }
  else
  {
    minBurst->endTime = currentTime + minTime;
    minBurst->latency = minBurst->endTime - minBurst->startTime;

    int IOIndex = minBurst->devices.at(0);
    IOQueue->readyQueue.at(IOIndex).push(minBurst);
    IOQueue->popReadyQueue();
  }

  //Set times
  currentTime += minTime;
  lastJobTime += minTime;
}

void Simulator::runIO()
{
  IOQueue->popReadyQueue();

  //Get smallest burst
  int minIO = IOQueue->getMinBurst();
  ProcessBurst *minBurst = IOQueue->queue.at(minIO).front();
  IOQueue->queue.at(minIO).pop_back();

  //Get time
  double minTime = minBurst->bursts.at(0);
  minBurst->bursts.erase(minBurst->bursts.begin());
  minBurst->devices.erase(minBurst->devices.begin());

  //FirstIO
  if (!minBurst->firstIO)
  {
    minBurst->firstIO = true;
    minBurst->responseTime = (minTime+currentTime) - minBurst->startTime;
  }

  //Lower time of tasks
  CPUQueue->lowerCPUTime(minTime);
  IOQueue->lowerIOTime(minTime);

  //Move to queue
  queue.push(minBurst);

  //Set time
  currentTime += minTime;
  lastJobTime += minTime;
}

void Simulator::moveTaskToCPU()
{
  while ((int)CPUQueue->queue.size() < CPUCount)
  {
    if (queue.empty())
    {
      break;
    }
    CPUQueue->queue.push_back(queue.front());
    queue.pop();
    currentTime += switchCost;
    lastJobTime += switchCost;
  }
}

void Simulator::clearQueues()
{
  //CPU QUEUE
  while (!CPUQueue->queue.empty())
  {
    queue.push(CPUQueue->queue.front());
    CPUQueue->queue.pop_back();
  }
  //IO QUEUE
  for (int i = 0; i < IOCount; i++)
  {
    while (!IOQueue->queue.at(i).empty())
    {
      queue.push(IOQueue->queue.at(i).back());
      IOQueue->queue.at(i).pop_back();
    }
  }
  //READY QUEUE
  for (int i = 0; i < IOCount; i++)
  {
    while (!IOQueue->readyQueue.at(i).empty())
    {
      queue.push(IOQueue->readyQueue.at(i).front());
      IOQueue->readyQueue.at(i).pop();
    }
  }
}

void Simulator::calcEfficiency()
{
  double CPUUsedCount = (double)CPUQueue->queue.size();
  double IOUsedCount = 0;

  for (int i = 0; i < IOCount; i++)
  {
    if (!IOQueue->queue.at(i).empty())
    {
      IOUsedCount++;
    }
  }
  double ratio = (CPUUsedCount + IOUsedCount) / (double)(CPUCount + IOCount);
  efficiency.push_back(ratio);
}

void Simulator::printStats()
{
  Stats *stats = new Stats(endTime, queue, completed, efficiency);

  std::cout << "-SETTINGS-" << std::endl;
  std::cout << " CPU Count: " << CPUCount << std::endl;
  std::cout << " IO  Count: " << IOCount << std::endl;
  std::cout << " Task Frequency: " << taskFrequency << std::endl;
  std::cout << " Switch Cost: " << switchCost << std::endl;
  std::cout << " Ending Time: " << endTime << std::endl << std::endl;

  std::cout << "-STATS-" << std::endl;
  std::cout << " Tasks: " << completed.size() << "/" << completed.size() + queue.size() << " (completed/created)" << std::endl;
  std::cout << " Throughput: " << stats->throughput << std::endl;

  std::cout << " LATENCY" << std::endl;
  std::cout << " Min: " << stats->minLatency << std::endl;
  std::cout << " Max: " << stats->maxLatency << std::endl;
  std::cout << " Avg: " << stats->avgLatency << std::endl;
  std::cout << " Dev: " << stats->latencyDeviation << std::endl;

  std::cout << " RESPONSE TIME" << std::endl;
  std::cout << " Min: " << stats->minResponseTime << std::endl;
  std::cout << " Max: " << stats->maxResponseTime << std::endl;
  std::cout << " Avg: " << stats->avgResponseTime << std::endl;
  std::cout << " Dev: " << stats->responseTimeDeviation << std::endl;

  std::cout << " EFFICIENCY" << std::endl;
  std::cout << " Min: " << stats->minEfficiency << std::endl;
  std::cout << " Max: " << stats->maxEfficiency << std::endl;
  std::cout << " Avg: " << stats->avgEfficiency << std::endl;
  std::cout << " Dev: " << stats->efficiencyDeviation << std::endl << std::endl;
}

