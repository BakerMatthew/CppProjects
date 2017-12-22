// Matt Baker, A01536520
// Program 7, Simulator - Part 1
// Created in: Raspberry Pi, Raspbian

#include "Simulator.hpp"

#include <iostream>

int main()
{
  Simulator *simulator;
  int CPUCount,
      IOCount;
  double switchCost,
         taskMix,
         taskFrequency,
         endTime;

  int response;
  bool continueSim = true;

  while (continueSim)
  {
    std::cout << "=================" << std::endl;
    std::cout << "=Simulation Menu=" << std::endl;
    std::cout << " -Custom Simulation: [1]" << std::endl;
    std::cout << " -Default Simulation: [2]" << std::endl;
    std::cout << " -Exit Simultaion: [3]" << std::endl;
    std::cout << "Input desired Simulation: ";
    std::cin >> response;
    std::cout << "=================" << std::endl << std::endl;

    switch (response)
    {
      //Custom Sim
      case 1:
        std::cout << "Number of CPUs: ";
        std::cin >> CPUCount;
        std::cout << "Number of IOs: ";
        std::cin >> IOCount;
        std::cout << "Cost of context switch: ";
        std::cin >> switchCost;
        std::cout << "Task mix of CPU vs IO bound task (0.xx): ";
        std::cin >> taskMix;
        std::cout << "Task frequency: ";
        std::cin >> taskFrequency;
        std::cout << "End time to end simulator: ";
        std::cin >> endTime;
        std::cout << std::endl << std::endl;

        simulator = new Simulator(switchCost, taskMix, taskFrequency, endTime, CPUCount, IOCount);
        simulator->beginSimulation();
        break;
      //Default Sim
      case 2:
        simulator = new Simulator();
        simulator->beginSimulation();
        break;
      //Exit Sim
      case 3:
        std::cout << "Exiting Simulator" << std::endl;
        continueSim = false;
        break;
      //Invalid input
      default:
        std::cout << "ERROR: The input " << response << " is not valid!" << std::endl;
        std::cout << "     : Valid inputs are the following integers: 1, 2, 3" << std::endl << std::endl;
    }
  }
}
