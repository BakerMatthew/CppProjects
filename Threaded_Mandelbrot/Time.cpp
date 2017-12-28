#include <chrono>
#include <vector>
#include <math.h>

//Returns how long function func took to execute
template <typename T>
int time(T func)
{
	//The code below to cast to an int was taken from stackoverflow user: Howard Hinnant
	typedef std::chrono::steady_clock Time;
	typedef std::chrono::duration<double> dSec;
	typedef std::chrono::milliseconds ms;

	auto startTime = Time::now();
	func();
	auto stopTime = Time::now();

	dSec time = stopTime - startTime;
	ms delta = std::chrono::duration_cast<ms>(time);

	return (int)delta.count();
}

//Returns a vector of two elements for the vect of functions (1)average (2)standard deviation
template <typename T>
std::vector<double> timeStats(std::vector<T> vect)
{
	std::vector<int> times;
	std::vector<double> timeInfo;
	double totalTime = 0;
	double average = 0;
	double variance = 0;

	//Calculate average
	for (int i = 0; i < (int)vect.size(); i++)
	{
		times.push_back(time(vect.at(i)));
		totalTime += times[i];
	}
	average = totalTime / (double)times.size();

	//Calculate variance
	for (int i = 0; i < (int)times.size(); i++)
	{
		variance += (times.at(i) - average)*(times.at(i) - average);
	}
	variance /= (double)times.size();

	//Push average and standard deviation into timeInfo
	timeInfo.push_back(average);
	timeInfo.push_back(sqrt(variance)); //Standard Deviation

	return timeInfo;
}