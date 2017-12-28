// Matt Baker - A01536520
// CS 3100    - Program 2 Threaded Mandelbrot Set
// Created in - Visual Studio 2015, Windows 10, jdk1.8.0_60

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <thread>

#include "Time.cpp"

const double MAX_R = 0.5;
const double MIN_R = -1.5;
const double MAX_I = 1.0;
const double MIN_I = -1.0;
const int WIDTH = 512;
const int HEIGHT = 512;
const int MAX_N = 512;
int rMult;
int gMult;
int bMult;
int threadC;

//vector size = WIDTH*HEIGHT*3 -> (512*512*3 = 786,432)
std::vector<int> mandelSet(WIDTH*HEIGHT*3);

double getRealValue(int value)
{
	return value * ((MAX_R - MIN_R) / WIDTH) + MIN_R;
}
double getImaginaryValue(int value)
{
	return value * ((MAX_I - MIN_I) / HEIGHT) + MIN_I;
}

int getMandelValue(double realVal, double imaginaryVal)
{
	int value = 0;

	for (double realTemp = 0, imaginaryTemp = 0; value < MAX_N && ((realTemp*realTemp)+(imaginaryTemp*imaginaryTemp)) < 4.0; value++)
	{
		double tempVal = (realTemp*realTemp) - (imaginaryTemp*imaginaryTemp) + realVal;
		imaginaryTemp = (2.0*realTemp*imaginaryTemp) + imaginaryVal;
		realTemp = tempVal;
	}
	return value;
}

/**
 * setRMult: the R multiplier value (red)
 * setGMult: the G multiplier value (green)
 * setBMult: the B multiplier value (blue)
 * threadCount: amount of threads
 */
std::vector<int> makeMandelbrot(int setRMult, int setGMult, int setBMult, int threadCount)
{
	threadC = threadCount;
	rMult = setRMult;
	gMult = setGMult;
	bMult = setBMult;

	//first thread (indexes of: 0 -> size*1/threadAmount)
	std::thread t1;
	if (threadCount >= 1)
	{
		std::thread thr1(
			[]() {
			for (int y = 0, count = 0; y < HEIGHT * 1/threadC; y++)
			{
				for (int x = 0; x < WIDTH; x++)
				{
					double realVal = getRealValue(x);
					double imaginaryVal = getImaginaryValue(y);

					int n = getMandelValue(realVal, imaginaryVal);

					int r = (n * rMult % 256);
					int g = (n * gMult % 256);
					int b = (n * bMult % 256);

					mandelSet.at(count++) = r;
					mandelSet.at(count++) = g;
					mandelSet.at(count++) = b;
				}
			}
		});
		t1.swap(thr1);
	}

	//second thread (indexes of: size*1/threadAmount -> size*2/threadAmount)
	std::thread t2;
	if (threadCount >= 2)
	{
		std::thread thr2(
			[]() {
			int count = HEIGHT*WIDTH*3 * 1/threadC;
			if (threadC % 2 != 0 || threadC == 6)
			{	//Need to check special cases incase of trunkated
				count = HEIGHT / threadC * WIDTH * 3;
			}

			for (int y = HEIGHT * 1/threadC; y < HEIGHT * 2/threadC; y++)
			{
				//std::cout << "y: " <<  y << " ";
				//std::cout << "count: " << count << " ";
				//std::cout << "y<: " << HEIGHT * 2 / threadC << " ";
				for (int x = 0; x < WIDTH; x++)
				{
					double realVal = getRealValue(x);
					double imaginaryVal = getImaginaryValue(y);

					int n = getMandelValue(realVal, imaginaryVal);

					int r = (n * rMult % 256);
					int g = (n * gMult % 256);
					int b = (n * bMult % 256);

					mandelSet.at(count++) = r;
					mandelSet.at(count++) = g;
					mandelSet.at(count++) = b;
				}
			}
		});
		t2.swap(thr2);
	}

	//third thread (indexes of: size*2/threadAmount -> size*3/threadAmount)
	std::thread t3;
	if (threadCount >= 3)
	{
		std::thread thr3(
			[]() {
			int count = HEIGHT*WIDTH * 3 * 2 / threadC;
			if (threadC % 2 != 0 || threadC == 6)
			{	//Need to check special cases incase of trunkated
				count = (HEIGHT / threadC * WIDTH * 3) + (((HEIGHT/threadC*2) - (WIDTH/threadC*1)) * WIDTH * 3);
			}

			for (int y = HEIGHT * 2/threadC; y < HEIGHT * 3/threadC; y++)
			{
				for (int x = 0; x < WIDTH; x++)
				{
					double realVal = getRealValue(x);
					double imaginaryVal = getImaginaryValue(y);

					int n = getMandelValue(realVal, imaginaryVal);

					int r = (n * rMult % 256);
					int g = (n * gMult % 256);
					int b = (n * bMult % 256);
					
					mandelSet.at(count++) = r;
					mandelSet.at(count++) = g;
					mandelSet.at(count++) = b;
				}
			}
		});
		t3.swap(thr3);
	}

	//fourth thread (indexes of: size*3/threadAmount -> size*4/threadAmount)
	std::thread t4;
	if (threadCount >= 4)
	{
		std::thread thr4(
			[]() {
			int count = HEIGHT*WIDTH * 3 * 3 / threadC;
			if (threadC % 2 != 0 || threadC == 6)
			{	//Need to check special cases incase of trunkated
				count = (HEIGHT / threadC * WIDTH * 3) + (((HEIGHT / threadC * 3) - (WIDTH / threadC * 1)) * WIDTH * 3);
			}

			for (int y = HEIGHT * 3/threadC; y < HEIGHT * 4/threadC; y++)
			{
				for (int x = 0; x < WIDTH; x++)
				{
					double realVal = getRealValue(x);
					double imaginaryVal = getImaginaryValue(y);

					int n = getMandelValue(realVal, imaginaryVal);

					int r = (n * rMult % 256);
					int g = (n * gMult % 256);
					int b = (n * bMult % 256);

					mandelSet.at(count++) = r;
					mandelSet.at(count++) = g;
					mandelSet.at(count++) = b;
				}
			}
		});
		t4.swap(thr4);
	}

	//fifth thread (indexes of: size*4/threadAmount -> size*5/threadAmount)
	std::thread t5;
	if (threadCount >= 5)
	{
		std::thread thr5(
			[]() {
			int count = HEIGHT*WIDTH * 3 * 4 / threadC;
			if (threadC % 2 != 0 || threadC == 6)
			{	//Need to check special cases incase of trunkated
				count = (HEIGHT / threadC * WIDTH * 3) + (((HEIGHT / threadC * 4) - (WIDTH / threadC * 1)) * WIDTH * 3);
			}

			for (int y = HEIGHT * 4/threadC; y < HEIGHT * 5/threadC; y++)
			{
				for (int x = 0; x < WIDTH; x++)
				{
					double realVal = getRealValue(x);
					double imaginaryVal = getImaginaryValue(y);

					int n = getMandelValue(realVal, imaginaryVal);

					int r = (n * rMult % 256);
					int g = (n * gMult % 256);
					int b = (n * bMult % 256);

					mandelSet.at(count++) = r;
					mandelSet.at(count++) = g;
					mandelSet.at(count++) = b;
				}
			}
		});
		t5.swap(thr5);
	}

	//sixth thread (indexes of: size*5/threadAmount -> size*6/threadAmount)
	std::thread t6;
	if (threadCount >= 6)
	{
		std::thread thr6(
			[]() {
			int count = HEIGHT*WIDTH * 3 * 5 / threadC;
			if (threadC % 2 != 0 || threadC == 6)
			{	//Need to check special cases incase of trunkated
				count = (HEIGHT / threadC * WIDTH * 3) + (((HEIGHT / threadC * 5) - (WIDTH / threadC * 1)) * WIDTH * 3);
			}

			for (int y = HEIGHT * 5/threadC; y < HEIGHT * 6/threadC; y++)
			{
				for (int x = 0; x < WIDTH; x++)
				{
					double realVal = getRealValue(x);
					double imaginaryVal = getImaginaryValue(y);

					int n = getMandelValue(realVal, imaginaryVal);

					int r = (n * rMult % 256);
					int g = (n * gMult % 256);
					int b = (n * bMult % 256);

					mandelSet.at(count++) = r;
					mandelSet.at(count++) = g;
					mandelSet.at(count++) = b;
				}
			}
		});
		t6.swap(thr6);
	}

	//seventh thread (indexes of: size*6/threadAmount -> size*7/threadAmount)
	std::thread t7;
	if (threadCount >= 7)
	{
		std::thread thr7(
			[]() {
			int count = HEIGHT*WIDTH * 3 * 6 / threadC;
			if (threadC % 2 != 0 || threadC == 6)
			{	//Need to check special cases incase of trunkated
				count = (HEIGHT / threadC * WIDTH * 3) + (((HEIGHT / threadC * 6) - (WIDTH / threadC * 1)) * WIDTH * 3);
			}

			for (int y = HEIGHT * 6/threadC; y < HEIGHT * 7/threadC; y++)
			{
				for (int x = 0; x < WIDTH; x++)
				{
					double realVal = getRealValue(x);
					double imaginaryVal = getImaginaryValue(y);

					int n = getMandelValue(realVal, imaginaryVal);

					int r = (n * rMult % 256);
					int g = (n * gMult % 256);
					int b = (n * bMult % 256);

					mandelSet.at(count++) = r;
					mandelSet.at(count++) = g;
					mandelSet.at(count++) = b;
				}
			}
		});
		t7.swap(thr7);
	}

	//eigth thread (indexes of: size*7/threadAmount -> size*8/threadAmount)
	std::thread t8;
	if (threadCount >= 8)
	{
		std::thread thr8(
			[]() {
			int count = HEIGHT*WIDTH * 3 * 7 / threadC;
			if (threadC % 2 != 0 || threadC == 6)
			{	//Need to check special cases incase of trunkated
				count = (HEIGHT / threadC * WIDTH * 3) + (((HEIGHT / threadC * 7) - (WIDTH / threadC * 1)) * WIDTH * 3);
			}

			for (int y = HEIGHT * 7/threadC; y < HEIGHT * 8/threadC; y++)
			{
				for (int x = 0; x < WIDTH; x++)
				{
					double realVal = getRealValue(x);
					double imaginaryVal = getImaginaryValue(y);

					int n = getMandelValue(realVal, imaginaryVal);

					int r = (n * rMult % 256);
					int g = (n * gMult % 256);
					int b = (n * bMult % 256);

					mandelSet.at(count++) = r;
					mandelSet.at(count++) = g;
					mandelSet.at(count++) = b;
				}
			}
		});
		t8.swap(thr8);
	}

	if (threadCount >= 1)
		t1.join();
	if (threadCount >= 2)
		t2.join();
	if (threadCount >= 3)
		t3.join();
	if (threadCount >= 4)
		t4.join();
	if (threadCount >= 5)
		t5.join();
	if (threadCount >= 6)
		t6.join();
	if (threadCount >= 7)
		t7.join();
	if (threadCount >= 8)
		t8.join();

	return mandelSet;
}

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

//These vectors will hold the mandelbrot image sets, used to write at the end of main()
std::vector<int> setOne;
std::vector<int> setTwo;
std::vector<int> setThree;
std::vector<int> setFour;
std::vector<int> setFive;
std::vector<int> setSix;

int main()
{
	std::cout << "Since writing the .ppm files takes a while . . .\n";
	std::cout << "I'll only write at the end of testing all eight threads!\n";
	std::cout << "(since writing isn't apart of the timing value)\n\n";

	//mandel: holds methods which will be timed by timeStats() from Time.cpp
	std::vector<void(*)()> mandel(6);
	//timing: holds the results of timeStats() from Time.cpp
	std::vector<double> timing;

	//Single Thread
	mandel[0] = ([]() { setOne	 = makeMandelbrot(1,  2,  3, 1); });
	mandel[1] = ([]() { setTwo	 = makeMandelbrot(2,  3,  1, 1); });
	mandel[2] = ([]() { setThree = makeMandelbrot(3,  2,  1, 1); });
	mandel[3] = ([]() { setFour	 = makeMandelbrot(5,  8, 11, 1); });
	mandel[4] = ([]() { setFive	 = makeMandelbrot(7, 13, 10, 1); });
	mandel[5] = ([]() { setSix	 = makeMandelbrot(9,  4,  4 ,1); });

	timing = timeStats(mandel);
	std::cout << "-Single Thread-" << std::endl;
	std::cout << "Average Time: " << timing.at(0) << " milliseconds" << std::endl;
	std::cout << "Standard Deviation: " << timing.at(1) << " milliseconds\n\n";
	
	//Two Threads
	mandel[0] = ([]() { setOne	 = makeMandelbrot(1,  2,  3, 2); });
	mandel[1] = ([]() { setTwo	 = makeMandelbrot(2,  3,  1, 2); });
	mandel[2] = ([]() { setThree = makeMandelbrot(3,  2,  1, 2); });
	mandel[3] = ([]() { setFour	 = makeMandelbrot(5,  8, 11, 2); });
	mandel[4] = ([]() { setFive	 = makeMandelbrot(7, 13, 10, 2); });
	mandel[5] = ([]() { setSix	 = makeMandelbrot(9,  4,  4, 2); });

	timing = timeStats(mandel);
	std::cout << "-Two Threads-" << std::endl;
	std::cout << "Average Time: " << timing.at(0) << " milliseconds" << std::endl;
	std::cout << "Standard Deviation: " << timing.at(1) << " milliseconds\n\n";
	
	//Three Threads
	mandel[0] = ([]() { setOne	 = makeMandelbrot(1,  2,  3, 3); });
	mandel[1] = ([]() { setTwo	 = makeMandelbrot(2,  3,  1, 3); });
	mandel[2] = ([]() { setThree = makeMandelbrot(3,  2,  1, 3); });
	mandel[3] = ([]() { setFour	 = makeMandelbrot(5,  8, 11, 3); });
	mandel[4] = ([]() { setFive	 = makeMandelbrot(7, 13, 10, 3); });
	mandel[5] = ([]() { setSix	 = makeMandelbrot(9,  4,  4, 3); });

	timing = timeStats(mandel);
	std::cout << "-Three Threads-" << std::endl;
	std::cout << "Average Time: " << timing.at(0) << " milliseconds" << std::endl;
	std::cout << "Standard Deviation: " << timing.at(1) << " milliseconds\n\n";
	
	//Four Threads
	mandel[0] = ([]() { setOne	 = makeMandelbrot(1,  2,  3, 4); });
	mandel[1] = ([]() { setTwo	 = makeMandelbrot(2,  3,  1, 4); });
	mandel[2] = ([]() { setThree = makeMandelbrot(3,  2,  1, 4); });
	mandel[3] = ([]() { setFour	 = makeMandelbrot(5,  8, 11, 4); });
	mandel[4] = ([]() { setFive	 = makeMandelbrot(7, 13, 10, 4); });
	mandel[5] = ([]() { setSix	 = makeMandelbrot(9,  4,  4, 4); });

	timing = timeStats(mandel);
	std::cout << "-Four Threads-" << std::endl;
	std::cout << "Average Time: " << timing.at(0) << " milliseconds" << std::endl;
	std::cout << "Standard Deviation: " << timing.at(1) << " milliseconds\n\n";
	
	//Five Threads
	mandel[0] = ([]() { setOne	 = makeMandelbrot(1,  2,  3, 5); });
	mandel[1] = ([]() { setTwo	 = makeMandelbrot(2,  3,  1, 5); });
	mandel[2] = ([]() { setThree = makeMandelbrot(3,  2,  1, 5); });
	mandel[3] = ([]() { setFour	 = makeMandelbrot(5,  8, 11, 5); });
	mandel[4] = ([]() { setFive	 = makeMandelbrot(7, 13, 10, 5); });
	mandel[5] = ([]() { setSix	 = makeMandelbrot(9,  4,  4, 5); });

	timing = timeStats(mandel);
	std::cout << "-Five Threads-" << std::endl;
	std::cout << "Average Time: " << timing.at(0) << " milliseconds" << std::endl;
	std::cout << "Standard Deviation: " << timing.at(1) << " milliseconds\n\n";
	
	//Six Threads
	mandel[0] = ([]() { setOne	 = makeMandelbrot(1,  2,  3, 6); });
	mandel[1] = ([]() { setTwo	 = makeMandelbrot(2,  3,  1, 6); });
	mandel[2] = ([]() { setThree = makeMandelbrot(3,  2,  1, 6); });
	mandel[3] = ([]() { setFour	 = makeMandelbrot(5,  8, 11, 6); });
	mandel[4] = ([]() { setFive	 = makeMandelbrot(7, 13, 10, 6); });
	mandel[5] = ([]() { setSix	 = makeMandelbrot(9,  4,  4, 6); });

	timing = timeStats(mandel);
	std::cout << "-Six Threads-" << std::endl;
	std::cout << "Average Time: " << timing.at(0) << " milliseconds" << std::endl;
	std::cout << "Standard Deviation: " << timing.at(1) << " milliseconds\n\n";

	//Seven Threads
	mandel[0] = ([]() { setOne	 = makeMandelbrot(1,  2,  3, 7); });
	mandel[1] = ([]() { setTwo	 = makeMandelbrot(2,  3,  1, 7); });
	mandel[2] = ([]() { setThree = makeMandelbrot(3,  2,  1, 7); });
	mandel[3] = ([]() { setFour	 = makeMandelbrot(5,  8, 11, 7); });
	mandel[4] = ([]() { setFive	 = makeMandelbrot(7, 13, 10, 7); });
	mandel[5] = ([]() { setSix	 = makeMandelbrot(9,  4,  4, 7); });

	timing = timeStats(mandel);
	std::cout << "-Seven Threads-" << std::endl;
	std::cout << "Average Time: " << timing.at(0) << " milliseconds" << std::endl;
	std::cout << "Standard Deviation: " << timing.at(1) << " milliseconds\n\n";

	//Eight Threads
	mandel[0] = ([]() { setOne	 = makeMandelbrot(1,  2,  3, 8); });
	mandel[1] = ([]() { setTwo	 = makeMandelbrot(2,  3,  1, 8); });
	mandel[2] = ([]() { setThree = makeMandelbrot(3,  2,  1, 8); });
	mandel[3] = ([]() { setFour	 = makeMandelbrot(5,  8, 11, 8); });
	mandel[4] = ([]() { setFive	 = makeMandelbrot(7, 13, 10, 8); });
	mandel[5] = ([]() { setSix	 = makeMandelbrot(9,  4,  4, 8); });

	timing = timeStats(mandel);
	std::cout << "-Eight Threads-" << std::endl;
	std::cout << "Average Time: " << timing.at(0) << " milliseconds" << std::endl;
	std::cout << "Standard Deviation: " << timing.at(1) << " milliseconds\n\n";


	//Write the .ppm files
	std::cout << "Writing .ppm files...\n";
	writeMandelbrot("imageOne.ppm", setOne);
	writeMandelbrot("imageTwo.ppm", setTwo);
	writeMandelbrot("imageThree.ppm", setThree);
	writeMandelbrot("imageFour.ppm", setFour);
	writeMandelbrot("imageFive.ppm", setFive);
	writeMandelbrot("imageSix.ppm", setSix);
	std::cout << "Done Writing!\n\n";
}