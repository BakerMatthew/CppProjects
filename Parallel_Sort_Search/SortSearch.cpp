// Matt Baker - A01536520
// CS 3100    - Program 4 Parallel Sort/Search
// Created in - Raspberry Pi, Raspbian

#include <iostream>
#include <vector>
#include <thread>
#include <functional>
#include <algorithm>
#include <atomic>

#include "Time.hpp"
#include "WorkQueue.hpp"

WorkQueue wQueue;
std::atomic<int> workDone(0);

void doStandardSort(std::vector<int> &data)
{
  std::sort(data.begin(), data.end());
}

void doStandardSearch(std::vector<int> data, int elementToFind)
{
  std::find(data.begin(), data.end(), elementToFind);
}

void sortPart(std::vector<int> &data, int leftSide, int rightSide)
{
  int leftIndex;
  int rightIndex;
  int swap;
  int pivot;
  const int LIMIT = 50;

  //Continue recursion until LIMIT elements
  while (rightSide - leftSide > LIMIT)
  {
    leftIndex = leftSide;
    rightIndex = rightSide;
    pivot = data.at((leftIndex + rightIndex) / 2);

    //Swap-Partition
    while (leftIndex <= rightIndex)
    {
      while (data.at(leftIndex) < pivot)
      {
        leftIndex++;
      }
      while (data.at(rightIndex) > pivot)
      {
        rightIndex--;
      }
      if (leftIndex <= rightIndex)
      {
        swap = data.at(leftIndex);
        data.at(leftIndex) = data.at(rightIndex);
        data.at(rightIndex) = swap;
        leftIndex++;
        rightIndex--;
      }
    }

    //Left is smaller => Recurse left
    if (leftIndex-leftSide < rightSide-rightIndex && leftSide < rightIndex)
    {
      workDone.fetch_add(1);
      wQueue.post( [&data, leftSide, rightIndex]()
      {
        sortPart(data, leftSide, rightIndex-1);
      });
      leftSide = leftIndex;
    }

    //Right is smaller => Recurse right
    else if (leftIndex < rightSide)
    {
      workDone.fetch_add(1);
      wQueue.post( [&data, leftIndex, rightSide]()
      {
        sortPart(data, leftIndex, rightSide);
      });
      rightSide = rightIndex - 1;
    }
  }

  //Sort last LIMIT elements
  std::sort(data.begin() + leftSide, data.begin() + (rightSide+1));

  workDone.fetch_sub(1);
}

void doMySort(std::vector<int> data, int threads)
{
  workDone.fetch_add(1);
  wQueue.post( [&data]()
  {
    sortPart(data, 0, data.size()-1);
  });

  wQueue.start(threads);

  while (workDone != 0)
  {
    //Wait to finish
  }
}

void doMySearch(std::vector<int> data, int elementToFind, int threads)
{
  WorkQueue workQueue;
  std::atomic<int> doneCount(0);
  int threadCount = 0;

  std::function<void(void)> searchPart = [&]()
  {
    const int THREAD_COUNT = ++threadCount;
    std::find(data.begin() + ((THREAD_COUNT-1)/threads*data.size()), data.begin() + (THREAD_COUNT/threads*data.size()), elementToFind);
    doneCount.fetch_add(1);
  };
  for (int i = 0; i < threads; i++)
  {
    workQueue.post(searchPart);
  }

  workQueue.start(threads);

  while (doneCount < threads)
  {
    //Wait to finish
  }
  workQueue.stopWork();
}

int main()
{
  int elementToFind;
  const int RUN_SIZE = 1;
  std::vector<double> timingData;
  std::vector<std::function<void(void)>> sortSearchFunctions(RUN_SIZE);

  for (int elements = 100; elements <= 1000000; elements *= 10)
  {
    //Fill data with elements in a simple descending order
    std::vector<int> data;
    for (int i = elements-1; i >= 0; i--)
    {
      data.push_back(i);
    }

    elementToFind = elements / 2;

    //Shuffle data
    //elementToFind = rand() % elements;
    //std::random_shuffle(data.begin(), data.end());

    //Test Sort&Search with std library and one thread
    std::vector<int> tempData = data;
    std::cout << "******************************************" << std::endl;
    std::cout << "-" << elements << " Elements-" << std::endl << std::endl;
    for (int threads = 1; threads == 1; threads++)
    {
      for (int i = 0; i < RUN_SIZE; i++)
      {
        sortSearchFunctions[i] = ([&]() { doStandardSort(tempData); tempData = data; });
      }

      timingData = timeStats(sortSearchFunctions);

      std::cout << "=" << threads << " Thread=" << std::endl;
      std::cout << " STD::SORT- " << timingData.at(0) << " microseconds" << std::endl;

      for (int i = 0; i < RUN_SIZE; i++)
      {
        sortSearchFunctions[i] = ([&]() { doStandardSearch(tempData, elementToFind); });
      }

      timingData = timeStats(sortSearchFunctions);

      std::cout << " STD::FIND- " << timingData.at(0) << " microseconds" << std::endl;
    }

    //Test Sort&Search with my own methods and 2-8 threads
    tempData = data;
    for (int threads = 2; threads <= 8; threads++)
    {
      for (int i = 0; i < RUN_SIZE; i++)
      {
        sortSearchFunctions[i] = ([&]() { doMySort(tempData, threads); });
      }

      timingData = timeStats(sortSearchFunctions);

      std::cout << "=" << threads << " Threads=" << std::endl;
      std::cout << " MY QUICKSORT- " << timingData.at(0) << " microseonds" << std::endl;

      for (int i = 0; i < RUN_SIZE; i++)
      {
        sortSearchFunctions[i] = ([&]() { doMySearch(tempData, elementToFind, threads); });
      }

      timingData = timeStats(sortSearchFunctions);

      std::cout << " MY LINEAR SEARCH- " << timingData.at(0) << " microseonds" << std::endl;
    }
    std::cout << "******************************************" << std::endl << std::endl;
  }
}
