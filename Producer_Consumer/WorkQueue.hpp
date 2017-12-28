#ifndef WORK_QUEUE_HPP
#define WORK_QUEUE_HPP

#include "ThreadSafeQueue.hpp"

#include <iostream>
#include <functional>
#include <atomic>
#include <vector>
#include <exception>
#include <thread>

class WorkQueue
{
public:
  WorkQueue() : threadPool(), taskList(), cont(true) {}
  ~WorkQueue() { stopWork(); }

  void start(int threadCount);
  void stopWork();
  void post(std::function<void(void)> func);

private:
  std::vector<std::thread> threadPool;
  ThreadSafeQueue<std::function<void(void)>> taskList;
  std::atomic<bool> cont;
};

#endif
