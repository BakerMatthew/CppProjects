#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <sstream>
#include <cstdio>
#include <iterator>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdexcept>

class Shell
{
public:
  Shell()
  {
    timeTaken = 0;
  }

  std::vector<std::string> history;
  double timeTaken;

  //For basic inputs
  void handleInput(std::vector<std::string> input);
  std::vector<std::vector<std::string>> parseInput(std::vector<std::string> input);

  //For piping and redirects
  void executeRedirect(std::vector<std::string> input);
  std::vector<std::string> parseForRedirect(std::string input);
  void execute(const char **arguments);

  //History and Time
  void showHistory();
  std::string findHistory(std::string element);
  void showTime();
};
