#include "CPUBurst.hpp"

CPUBurst::CPUBurst(int CPUCount)
{
  this->CPUCount = CPUCount;
}

//Return index of lowest burst
int CPUBurst::getMinBurst()
{
  int min = 0;
  int currIndex = 0;
  for (ProcessBurst *curr : queue)
  {
    if (queue.at(min)->bursts.at(0) > curr->bursts.at(0))
    {
      min = currIndex;
    }
    currIndex++;
  }
  return min;
}

bool CPUBurst::notEmpty()
{
  return !queue.empty(); 
}

//Lower all CPUTime in queue by time
void CPUBurst::lowerCPUTime(double time)
{
  for (ProcessBurst *curr : queue)
  {
    curr->bursts[0] -= time;
  }
}
