#ifndef THREAD_SAFE_QUEUE_HPP
#define THREAD_SAFE_QUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <exception>

template <typename T>
class ThreadSafeQueue
{
public:
  ThreadSafeQueue() : data(), stopQueue(false), mutex_data(), notEmpty() {}

  void enqueue(T item)
  {
    std::unique_lock<std::mutex> l(mutex_data);
    data.push(item);
    notEmpty.notify_all();
  }

  T dequeue()
  {
    std::unique_lock<std::mutex> l(mutex_data);
    while (data.empty())
    {
      notEmpty.wait(l);
      if (stopQueue)
      {
        throw std::runtime_error("wait aborted\n");
      }
    }
    auto result = data.front();
    data.pop();
    return result;
  }

  //Throws all blocked queues
  void abort()
  {
    stopQueue = true;
    notEmpty.notify_all();
  }

  void begin()
  {
    stopQueue = false;
  }

private:
  std::queue<T> data;
  std::atomic<bool> stopQueue;
  std::mutex mutex_data;
  std::condition_variable notEmpty;
};

#endif
