#include "Stats.hpp"

Stats::Stats(double endTime, std::queue<ProcessBurst*> queue, std::vector<ProcessBurst*> completed, std::vector<double> efficiency)
{
  this->endTime = endTime;
  this->queue = queue;
  this->completed = completed;
  this->efficiency = efficiency;

  computeStats();
}

void Stats::computeStats()
{
  //THROUGHPUT
  throughput = (double)completed.size() / endTime;

  //LATENCY
  std::vector<double> latencyMean;
  double latencyMeanTotal = 0;
  for (ProcessBurst *curr : completed)
  {
    avgLatency += curr->latency;
    if (curr->latency > maxLatency)
    {
      maxLatency = curr->latency;
    }
    if (curr->latency < minLatency)
    {
      minLatency = curr->latency;
    }
  }
  if (completed.size() > 0)
  {
    avgLatency /= (double)completed.size();
  }
  for (ProcessBurst *curr : completed)
  {
    latencyMean.push_back(pow(curr->latency - avgLatency, 2));
  }
  for (double curr : latencyMean)
  {
    latencyMeanTotal += curr;
  }
  if (latencyMean.size() > 0)
  {
    latencyMeanTotal /= (double)latencyMean.size();
  }
  latencyDeviation = sqrt(latencyMeanTotal);
  if (avgLatency == 0)
  {
    minLatency = 0;
    maxLatency = 0;
  }

  //RESPONSE TIME
  int responseTimeSize = 0;
  std::vector<double> responseTimeMean;
  double responseTimeMeanTotal = 0;
  for (ProcessBurst *curr : completed)
  {
    avgResponseTime += curr->responseTime;
    if (curr->responseTime > maxResponseTime)
    {
      maxResponseTime = curr->responseTime;
    }
    if (curr->responseTime < minResponseTime)
    {
      curr->responseTime = curr->responseTime;
    }
    responseTimeMean.push_back(curr->responseTime);
  }
  std::vector<ProcessBurst*> finished;
  for (int i = 0; i < (int)queue.size(); i++)
  {
    ProcessBurst *curr = queue.front();
    if (curr->firstIO)
    {
      finished.push_back(curr);
    }
    queue.pop();
    queue.push(curr);
  }
  for (ProcessBurst *curr : finished)
  {
    if (curr->firstIO)
    {
      responseTimeSize++;
      avgResponseTime += curr->responseTime;
      if (curr->responseTime > maxResponseTime)
      {
        maxResponseTime = curr->responseTime;
      }
      if (curr->responseTime < minResponseTime)
      {
        minResponseTime = curr->responseTime;
      }
      responseTimeMean.push_back(curr->responseTime);
    }
  }
  avgResponseTime = avgResponseTime / (double)(responseTimeSize + completed.size());

  std::vector<double> rtMean;
  for (double curr : responseTimeMean)
  {
    rtMean.push_back(pow(curr-avgResponseTime, 2));
  }
  for (double curr : rtMean)
  {
    responseTimeMeanTotal += curr;
  }
  responseTimeMeanTotal /= (double)rtMean.size();
  responseTimeDeviation = sqrt(responseTimeMeanTotal);

  //EFFICIENCY
  std::vector<double> efficiencyMean;
  double efficiencyMeanTotal = 0;
  for (double curr : efficiency)
  {
    avgEfficiency += curr;
    if (curr > maxEfficiency)
    {
      maxEfficiency = curr;
    }
    if (curr < minEfficiency)
    {
      minEfficiency = curr;
    }
    efficiencyMean.push_back(curr);
  }
  avgEfficiency /= (double)efficiency.size();
  avgEfficiency *= 100;
  minEfficiency *= 100;
  maxEfficiency *= 100;

  std::vector<double> eMean;
  for (double curr : efficiencyMean)
  {
    eMean.push_back(pow(curr-avgEfficiency, 2));
  }
  for (double curr : eMean)
  {
    efficiencyMeanTotal += curr;
  }
  efficiencyMeanTotal /= (double)eMean.size();
  efficiencyDeviation = sqrt(efficiencyMeanTotal);
}
