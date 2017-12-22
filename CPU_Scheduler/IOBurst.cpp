#include "IOBurst.hpp"

IOBurst::IOBurst(int IOCount)
{
  this->IOCount = IOCount;
  queue.resize(IOCount);
  readyQueue.resize(IOCount);
}

//Returns index of lowest IOburst
int IOBurst::getMinBurst()
{
  int min = 0;
  for (int i = 0; i < IOCount; i++)
  {
    if (!queue.at(i).empty())
    {
      min = i;
    }
  }
  for (int i = 0; i < IOCount; i++)
  {
    if (!queue.at(i).empty())
    {
      if (queue.at(min).at(0)->bursts.at(0) > queue.at(i).at(0)->bursts.at(0))
      {
        min = i;
      }
    }
  }
  return min;
}

bool IOBurst::notEmpty()
{
  for (int i = 0; i < IOCount; i++)
  {
    if (!queue.at(i).empty())
    {
      return true;
    }
  }
  return false;
}

//Lower all IOtime in queue by time
void IOBurst::lowerIOTime(double time)
{
  for (int i = 0; i < IOCount; i++)
  {
    if (!queue.at(i).empty())
    {
      queue.at(i).at(0)->bursts.at(0) -= time;
    }
  }
}

//Move burst from readyQueue to queue
void IOBurst::popReadyQueue()
{
  for (int i = 0; i < IOCount; i++)
  {
    if (!readyQueue.at(i).empty() && queue.at(i).empty())
    {
      queue.at(i).push_back(readyQueue.at(i).front());
      readyQueue.at(i).pop();
    }
  }
}
