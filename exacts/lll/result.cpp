#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>

int main(int argc, char *argv[])
{
  int edgeCount = std::stoi(std::string(argv[1]));

  std::vector<std::vector<int>> solutions;

  std::string line;
  while (std::getline(std::cin, line))
  {
    line.erase(std::remove(line.begin(), line.end(), '['), line.end());
    line.erase(std::remove(line.begin(), line.end(), ']'), line.end());

    std::stringstream lineStream;
    lineStream << line;

    std::vector<int> solution(edgeCount);

    std::string value;
    int edge = 0;
    bool isValid = true;

    int firstValue = -100;

    while (std::getline(lineStream, value, ' '))
    {
      int intValue = std::stoi(value);
      if (edge < edgeCount)
      {
        if (firstValue == -100 && (intValue == 1 || intValue == -1))
        {
          firstValue = intValue;
        }

        solution[edge] = intValue;
        isValid &= (intValue == 0 || (firstValue == -1 ? intValue == -1 : intValue == 1));
        edge++;
      }
      else
      {
        isValid &= intValue == 0;
      }
    }

    if (isValid && edge > 0)
    {
      solutions.push_back(solution);
    }
  }

  int bestSolutionSize = 0;
  std::vector<int> bestSolution;

  for (int i = 0; i < solutions.size(); i++)
  {
    int size = 0;
    for (int j = 0; j < solutions[i].size(); j++)
    {
      size += std::abs(solutions[i][j]);
    }

    if (size > bestSolutionSize)
    {
      bestSolutionSize = size;
      bestSolution = solutions[i];
    }
  }

  std::cout << bestSolutionSize << std::endl;
  // for (int j = 0; j < bestSolution.size(); j++)
  // {
  //   std::cout << bestSolution[j] << " ";
  // }
  // std::cout << std::endl;

  return 0;
}
