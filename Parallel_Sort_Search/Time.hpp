#ifndef TIME_HPP
#define TIME_HPP

#include <chrono>
#include <vector>
#include <math.h>

template <typename T>
int time(T func)
{
  typedef std::chrono::steady_clock Time;
  typedef std::chrono::duration<double> dSec;
  typedef std::chrono::microseconds ms;

  auto startTime = Time::now();
  func();
  auto stopTime = Time::now();

  dSec time = stopTime - startTime;
  ms delta = std::chrono::duration_cast<ms>(time);

  return (int)delta.count();
}

template <typename T>
std::vector<double> timeStats(std::vector<T> vect)
{
  std::vector<int> times;
  std::vector<double> timeInfo;
  double totalTime = 0;
  double average = 0;
  double variance = 0;

  for (int i = 0; i < (int)vect.size(); i++)
  {
    times.push_back(time(vect.at(i)));
    totalTime += times.at(i);
  }
  average = totalTime / (double)times.size();

  for (int i = 0; i < (int)times.size(); i++)
  {
    variance += (times.at(i) - average)*(times.at(i) - average);
  }
  variance /= (double)times.size();

  timeInfo.push_back(average);
  timeInfo.push_back(sqrt(variance));

 return timeInfo;
}

template <typename T>
int getTime(T func)
{
  return time(func);
}

#endif
