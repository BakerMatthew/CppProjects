#include "Shell.hpp"

void Shell::handleInput(std::vector<std::string> input)
{
  auto pid = fork();
  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();

  if(pid == -1)
  {
    std::cerr << "ERROR: unable to fork";
  }
  //Child
  else if (pid == 0)
  {
    const char **arguments = new const char*[input.size()+1]; // +1 for NULL
    for (int i = 0; i < (int)input.size(); ++i)
    {
      arguments[i] = input.at(i).c_str();
    }
    arguments[input.size()] = NULL;

    std::cerr << "      ";
    if (execvp(arguments[0], (char **)arguments) < 0)
    {
      std::cerr << "ERROR: execvp couldn't execute your command" << std::endl;
    }
    exit(0);
  }
  //Parent
  else
  {
    waitpid(pid, nullptr, 0);

    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    timeTaken = elapsed_seconds.count();
  }
}

std::vector<std::string> Shell::parseForRedirect(std::string input)
{
  std::istringstream buffer(input);
  std::istream_iterator<std::string> beginning(buffer), end;
  std::vector<std::string> parts(beginning, end);

  return parts;
}


void Shell::execute(const char **arguments)
{
  auto pid = fork();
  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();
  if(pid == -1)
  {
    std::cerr << "ERROR: unable to fork";
  }
  //Child
  else if (pid == 0)
  {
    execvp(arguments[0], (char **)arguments);
    exit(0);
  }
  //Parent
  else
  {
    waitpid(pid, nullptr, 0);

    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    timeTaken = elapsed_seconds.count();
  }
}

void Shell::showHistory()
{
  int index = 1;
  std::cout << "History:" << std::endl;
  for(auto s : history)
  {
    std::cout << "      ";
    std::cout << "(" << index << ") " << s << std::endl;
    index++;
  }
}

std::string Shell::findHistory(std::string str)
{
  str.erase(0, 1);
  std::string userInput;
  int element = atoi(str.c_str());

  if(element <= (int)history.size() && element > 0)
  {
    userInput = history[element-1];
  }
  else
  {
    userInput = "error";
  }

  return userInput;
}

void Shell::showTime()
{
  int seconds = (int)timeTaken;
  int milliseconds = (int) ((timeTaken - seconds) * 1000);
  int microseconds = (((timeTaken - seconds) * 1000) - milliseconds) * 1000;

  std::cout << "      ";
  std::cout << "Time spent executing: "
            << seconds << " seconds "
            << milliseconds << " milliseconds and "
            << microseconds << " microseconds."
            << std::endl;
}

std::vector<std::vector<std::string>> Shell::parseInput(std::vector<std::string> input)
{
  std::vector<std::vector<std::string>> parsed;
  std::vector<std::string> temp;

  std::string lastElement;
  for(std::string element : input)
  {
    if(element != "|" && element != "<" && element != ">")
    {
      temp.push_back(element);
    }
    else
    {
      parsed.push_back(temp);
      temp.clear();
      std::vector<std::string> redirect;
      redirect.push_back(element);
      parsed.push_back(redirect);
    }
    lastElement = element;
  }
  std::vector<std::string> lastChunk;
  lastChunk.push_back(lastElement);
  parsed.push_back(lastChunk);

  return parsed;
}

void Shell::executeRedirect(std::vector<std::string> input)
{
  std::vector<std::vector<std::string>> parsed = parseInput(input);
  int oldPipe[2];
  int newPipe[2];
  pipe(oldPipe);
  pipe(newPipe);
  int curr = 0;
  int newCurr = 0;
  const int STD_IN = 0;
  const int STD_OUT = 1;

  while(curr < (int)parsed.size())
  {
    if(curr == (int)parsed.size() - 1)
    {
      newCurr = curr + 4;
    }
    else if(parsed[curr + 1][0] == "|")
    {
      newCurr = curr + 2;
      pipe(newPipe);
    }
    else
    {
      newCurr = curr + 4;
    }

    //BEGIN FORK
    auto pid = fork();
    if(pid == -1)
    {
      std::cerr << "ERROR: unable to fork";
    }
    //Child
    else if (pid == 0)
    {
      if (curr == (int)parsed.size() - 1)
      {
        dup2(oldPipe[0], STD_IN);
        close(oldPipe[0]);
        close(oldPipe[1]);
      }
      //Read from file
      else if(parsed[curr + 1][0] == "<")
      {
        auto inFile = open(parsed[curr + 2][0].c_str(), O_RDWR);
        dup2(inFile, 0);
        close(inFile);
      }
      //Output to file
      else if(parsed[curr + 1][0] == ">")
      {
        auto outFile = open(parsed[curr + 2][0].c_str(), O_APPEND | O_WRONLY);
        dup2(outFile, 1);
        close(outFile);
      }
      //Pipe
      else
      {
        //Read previous execution
        if(curr != 0)
        {
          dup2(oldPipe[0], STD_IN);
          close(oldPipe[0]);
          close(oldPipe[1]);
        }
        //Write to next execution
        if (curr != (int)parsed.size() - 1)
        {
          close(newPipe[0]);
          dup2(newPipe[1], STD_OUT);
          close(newPipe[1]);
        }
      }

      const char **arguments = new const char*[parsed[curr].size()+1]; // +1 for NULL
      for (int i = 0; i < (int)parsed[curr].size(); ++i)
      {
        arguments[i] = parsed[curr].at(i).c_str();
      }
      arguments[parsed[curr].size()] = NULL;

      if (execvp(arguments[0], (char**)arguments) < 0)
      {
        std::cerr << "      ";
        std::cerr << "ERROR: execvp couldn't execute your command" << std::endl;
        _exit(1);
      }
    }
    //Parent
    else
    {
      //Close pipes from previous execution
      if(curr != 0)
      {
        close(oldPipe[0]);
        close(oldPipe[1]);
      }
      //Reset pipes for next execution
      if (curr != (int)parsed.size() - 1)
      {
        oldPipe[0] = newPipe[0];
        oldPipe[1] = newPipe[1];
      }
      waitpid(pid, nullptr, 0);
    }
    curr = newCurr;
  }
}
