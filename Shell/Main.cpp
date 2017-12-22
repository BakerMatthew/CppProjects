// Matt Baker, A01536520
// Program 6, Shell Program - Part 2 - Redirection
// Created in: Raspberry Pi, Raspbian

#include "Shell.hpp"
#include <iostream>
#include <string>

int main()
{
  Shell* myShell = new Shell();
  bool cont = true;

  while (cont)
  {
    std::cout << "[cmd]: ";
    std::string input;
    std::getline(std::cin, input);

    //PTIME
    if (input == "ptime")
    {
       myShell->history.push_back(input);
       myShell->showTime();
    } 
    //HISTORY
    else if (input == "history")
    {
      myShell->showHistory();
    }
    //EXECUTE HISTORY
    else if (input[0] == '^')
    {
      
      std::string recall = myShell->findHistory(input);
      if(recall == "error")
      {
         std::cerr << "ERROR: The given history doesn't exist" << std::endl;
      }
      else
      {
        //Redirect command
        if(recall.find('|') != std::string::npos ||
           recall.find('<') != std::string::npos ||
           recall.find('>') != std::string::npos)
        {
           std::vector<std::string> vectorOfInput = myShell->parseForRedirect(recall);
           myShell->executeRedirect(vectorOfInput);
        }
        //Normal command
        else
        {
           std::vector<std::string> vectorOfInput = myShell->parseForRedirect(recall);
           myShell->handleInput(vectorOfInput);
        }
      }
    }
    //EXIT
    else if (input == "exit")
    {
      cont = false;
      continue;
    }
    //NORMAL EXECUTION
    else
    {
      myShell->history.push_back(input);
      //Redirect command
      if(input.find('|') != std::string::npos ||
         input.find('<') != std::string::npos ||
         input.find('>') != std::string::npos)
      {
        std::vector<std::string> vectorOfInput = myShell->parseForRedirect(input);
        myShell->executeRedirect(vectorOfInput);
      }
      //Normal command
      else
      {
        std::vector<std::string> vectorOfInput = myShell->parseForRedirect(input);
        myShell->handleInput(vectorOfInput);
      }
    }
  }
}
