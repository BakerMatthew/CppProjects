// Matt Baker - A01536520
// CS 3100    - Program 3 Producer/Consumer
// Created in - Raspberry Pi, Raspbian

#include "Time.hpp"
#include "WorkQueue.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <atomic>


const double MAX_R = 0.5;
const double MIN_R = -1.5;
const double MAX_I = 1.0;
const double MIN_I = -1.0;
const int WIDTH = 512;
const int HEIGHT = 512;
const int MAX_N = 512;
int threadC;

int splitSize = 100;
int remainderSplitSize = (HEIGHT % ((HEIGHT / splitSize) * splitSize)) / (HEIGHT / splitSize);

std::vector<int> mandelSet(WIDTH*HEIGHT*3);
std::vector<int> setOne;
std::vector<int> setTwo;

void writeMandelbrot(std::string imageName, std::vector<int> set)
{
  std::ofstream write;
  write.open(imageName);

  write << "P3" << std::endl;
  write << WIDTH << " " << HEIGHT << std::endl;
  write << "256" << std::endl;

  for (int i = 0; i < (int)set.size(); i++)
  {
    write << set.at(i) << " ";
  }
  write.close();
}

double getRealValue(const int VALUE)
{
  return VALUE * ((MAX_R - MIN_R) / WIDTH) + MIN_R;
}

double getImaginaryValue(const int VALUE)
{
  return VALUE * ((MAX_I - MIN_I) / HEIGHT) + MIN_I;
}

int getMandelValue( const double REAL_VAL, const double IMAGINARY_VAL)
{
  int value = 0;

  for (double realTemp = 0, imaginaryTemp = 0; value < MAX_N && ((realTemp*realTemp) + (imaginaryTemp * imaginaryTemp)) < 4.0; value++)
  {
    double tempVal = (realTemp*realTemp) - (imaginaryTemp*imaginaryTemp) + REAL_VAL;
    imaginaryTemp = (2.0*realTemp*imaginaryTemp) + IMAGINARY_VAL;
    realTemp = tempVal;
  }
  return value;
}

void makeMandelbrotPixelChunk(const int PIXEL_COUNT, const int PIXEL_SIZE)
{
  int count = PIXEL_COUNT * PIXEL_SIZE * 3;
  int yCount = (PIXEL_COUNT * PIXEL_SIZE) / HEIGHT;
  int xCount = ((PIXEL_COUNT * PIXEL_SIZE) - (yCount * HEIGHT));

  for (int i = 0; i < PIXEL_SIZE; i++, xCount++)
  {
    double realVal = getRealValue(xCount);
    double imaginaryVal = getImaginaryValue(yCount);
    int baseVal = getMandelValue(realVal, imaginaryVal);

    int red = (baseVal * 9 % 256);
    int green = (baseVal * 3 % 256);
    int blue = (baseVal * 5 % 256);

    mandelSet.at(count++) = red;
    mandelSet.at(count++) = green;
    mandelSet.at(count++) = blue;
  }
}

std::vector<int> makeMandelPixels(const int THREAD_COUNT, const int PIXEL_SIZE)
{
  WorkQueue wQueue;
  const int PIXEL_AMOUNT = WIDTH * HEIGHT / PIXEL_SIZE;
  std::atomic<int> doneCount(0);

  std::function<void(void)> makePart = [&doneCount, PIXEL_SIZE]()
  {
    doneCount.fetch_add(1);
    const int COUNT = doneCount;
    makeMandelbrotPixelChunk(COUNT, PIXEL_SIZE);
  };

  for (int i = 0; i < PIXEL_AMOUNT; i++)
  {
    wQueue.post(makePart);
  }

  wQueue.start(THREAD_COUNT);
  while (doneCount < PIXEL_AMOUNT)
  {
    //Wait to finish
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(5));

  return mandelSet;
}

void makeMandelbrotEqualChunk(const int THREAD_RUN_COUNT)
{
  int count = (HEIGHT / threadC * WIDTH * 3) + (((HEIGHT / threadC * (THREAD_RUN_COUNT - 1)) - (WIDTH / threadC * 1)) * WIDTH * 3);

  for (int y = HEIGHT * (THREAD_RUN_COUNT - 1) / threadC; y < HEIGHT * THREAD_RUN_COUNT / threadC; y++)
  {
    for (int x = 0; x < WIDTH; x++)
    {
      double realVal = getRealValue(x);
      double imaginaryVal = getImaginaryValue(y);
      int baseVal = getMandelValue(realVal, imaginaryVal);

      int red = (baseVal * 9 % 256);
      int green = (baseVal * 3 % 256);
      int blue = (baseVal * 5 % 256);

      mandelSet.at(count++) = red;
      mandelSet.at(count++) = green;
      mandelSet.at(count++) = blue;
    }
  }
}

std::vector<int> makeMandelEqualChunks(const int THREAD_COUNT)
{
  WorkQueue wQueue;
  const int RUN_AMOUNT = splitSize + remainderSplitSize;
  std::atomic<int> doneCount(0);
  threadC = RUN_AMOUNT;

  std::function<void(void)> makePart = [&doneCount]()
  {
    doneCount.fetch_add(1);
    const int COUNT = doneCount;
    makeMandelbrotEqualChunk(COUNT);
  };

  for (int i = 0; i < RUN_AMOUNT; i++)
  {
    wQueue.post(makePart);
  }

  wQueue.start(THREAD_COUNT);

  while (doneCount < RUN_AMOUNT)
  {
    //Wait to finish
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  return mandelSet;
}

int main()
{
  std::vector<std::function<void(void)>> mandel(2);
  std::vector<double> timing;
  int pixelSize;

  // one pixel
  std::cout << "ONE PIXEL" << std::endl;
  pixelSize = 1;
  for (int threads = 1; threads <= 8; threads++)
  {
    mandel[0] = ([&]() { const int COUNT = threads; setOne = makeMandelPixels(COUNT, pixelSize); });
    mandel[1] = ([&]() { const int COUNT = threads; setTwo = makeMandelPixels(COUNT, pixelSize); });

    timing = timeStats(mandel);
    std::cout << "-" << threads << " Thread(s)-" << std::endl;
    std::cout << "Average Time: " << timing.at(0) << " milliseconds" << std::endl;
    std::cout << "Standard Deviation: " << timing.at(1) << " milliseconds" << std::endl;
  }

  // multiple pixels but less than one row
  std::cout << "MULTIPLE PIXELS < ONE ROW" << std::endl;
  pixelSize = 16;
  for (int threads = 1; threads <= 8; threads++)
  {
    mandel[0] = ([&]() { const int COUNT = threads; setOne = makeMandelPixels(COUNT, pixelSize); });
    mandel[1] = ([&]() { const int COUNT = threads; setTwo = makeMandelPixels(COUNT, pixelSize); });

    timing = timeStats(mandel);
    std::cout << "-" << threads << " Thread(s)-" << std::endl;
    std::cout << "Average Time: " << timing.at(0) << " milliseconds" << std::endl;
    std::cout << "Standard Deviation: " << timing.at(1) << " milliseconds" << std::endl;
  }

  // one row
  std::cout << "ONE ROW" << std::endl;
  splitSize = HEIGHT;
  for (int threads = 1; threads <= 8; threads++)
  {
    mandel[0] = ([&]() { const int COUNT = threads; setOne = makeMandelEqualChunks(COUNT); });
    mandel[1] = ([&]() { const int COUNT = threads; setTwo = makeMandelEqualChunks(COUNT); });

    timing = timeStats(mandel);
    std::cout << "-" << threads << " Thread(s)-" << std::endl;
    std::cout << "Average Time: " << timing.at(0) << " milliseconds" << std::endl;
    std::cout << "Standard Deviation: " << timing.at(1) << " milliseconds" << std::endl;
  }

  // differing chunks
  std::cout << "DIFFERING CHUNKS" << std::endl;
  pixelSize = 512;
  for (int threads = 1; threads <= 8; threads++)
  {
    mandel[0] = ([&]() { const int COUNT = threads; setOne = makeMandelPixels(COUNT, pixelSize); });
    mandel[1] = ([&]() { const int COUNT = threads; setTwo = makeMandelPixels(COUNT, pixelSize); });

    timing = timeStats(mandel);
    std::cout << "-" << threads << " Thread(s)-" << std::endl;
    std::cout << "Average Time: " << timing.at(0) << " milliseconds" << std::endl;
    std::cout << "Standard Deviation: " << timing.at(1) << " milliseconds" << std::endl;
  }

  // rows/n size chunks
  std::cout << "EQUAL CHUNKS" << std::endl;
  splitSize = 100;
  for (int threads = 1; threads <= 8; threads++)
  {
    mandel[0] = ([&]() { const int COUNT = threads; setOne = makeMandelEqualChunks(COUNT); });
    mandel[1] = ([&]() { const int COUNT = threads; setTwo = makeMandelEqualChunks(COUNT); });

    timing = timeStats(mandel);
    std::cout << "-" << threads << " Thread(s)-" << std::endl;
    std::cout << "Average Time: " << timing.at(0) << " milliseconds" << std::endl;
    std::cout << "Standard Deviation: " << timing.at(1) << " milliseconds" << std::endl;
  }

  std::cout << "Writing .ppm files..." << std::endl;
  writeMandelbrot("mandelImageOne.ppm", setOne);
  writeMandelbrot("mandelImageTwo.ppm", setTwo);
  std::cout << "Done Writing!" << std::endl << std::endl;
}
