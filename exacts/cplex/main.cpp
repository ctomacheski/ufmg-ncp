#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <chrono>
#include <cmath>

#include "cplex.h"
#include "../../util/pre-processing.h"

int main(int argc, char **argv)
{
  int useLB = 0;
  if (argc == 2)
  {
    useLB = std::stoi(std::string(argv[1]));
  }

  int vertexCount;
  int edgeCount;
  std::cin >> vertexCount >> edgeCount;

  Graph graph = parseIncidenceMatrix(vertexCount, edgeCount);

  // std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  preProcess(graph);
  // std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

  // int excludedNodes = 0;
  // for (int i = 0; i < vertexCount; i++)
  // {
  //   bool excluded = graph.ingoingEdges[i].size() == 0 && graph.outgoingEdges[i].size() == 0;
  //   if (excluded)
  //   {
  //     excludedNodes++;
  //   }
  // }

  // long allMaxWeight = 0;
  // for (int i = 0; i < graph.incidenceMatrix.size(); i++)
  // {
  //   allMaxWeight = std::max(allMaxWeight, graph.incidenceMatrix[i].weight);
  // }

  // double density = ((double)graph.incidenceMatrix.size()) / ((vertexCount - excludedNodes) * std::log2(allMaxWeight));

  // std::cout << "density " << density << std::endl;
  // std::cout << "time " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
  //           << " nodes " << vertexCount - excludedNodes << " edges " << graph.incidenceMatrix.size() << std::endl;

  Result result = nilcatenationCplex(graph, vertexCount, 3600, useLB == 1);

  std::cout
      << result.objectiveValue << " "
      << result.gap << " "
      << (result.isOptimal ? "1" : "0") << " "
      << (verifySolution(graph.incidenceMatrix, result, vertexCount) ? "1" : "0")
      << std::endl;

  std::cout << std::endl;
}
