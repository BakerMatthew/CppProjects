#include "WorkQueue.hpp"

void WorkQueue::start(int threadCount)
{
  cont = true;
  taskList.begin();

  auto doWork = [&]()
  {
    try
    {
      while (cont)
      {
        auto func = taskList.dequeue();
        try
        {
          func();
        }
        catch (...)
        {
          //Do nothing
        }
      }
    }
//    catch (std::exception& message)
//    {
//      std::cout << message.what();
//    }
    catch (...)
    {
      //Do nothing
    }
  };

  for (int i = 0; i < threadCount; i++)
  {
    threadPool.emplace_back(doWork);
  }
}

void WorkQueue::stopWork()
{
  cont = false;
  taskList.abort();
  for (auto&& t : threadPool)
  {
    t.join();
  }
  threadPool.clear();
}

void WorkQueue::post(std::function<void(void)> func)
{
  taskList.enqueue(func);
}
