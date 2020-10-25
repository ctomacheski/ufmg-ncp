#include <iostream>
#include <vector>

#include "cplex.h"
#include "../../util/subset.h"

std::set<int> findNonUsefulNodes(int vertexCount, Graph graph) {
  std::set<int> toExclude = std::set<int>();
  for (int i = 0; i < vertexCount; i++) {
    std::vector<int> subsetSumInput;
    for (auto it = graph.ingoingEdges[i].begin(); it != graph.ingoingEdges[i].end(); it++) {
        subsetSumInput.push_back(it->weight);
    }
    for (auto it = graph.outgoingEdges[i].begin(); it != graph.outgoingEdges[i].end(); it++) {
        subsetSumInput.push_back(-it->weight);
    }

    bool hasSubsetSum = subsetSum(subsetSumInput, 0);

    if (!hasSubsetSum) {
      toExclude.insert(i);
    }
  }

  std::cout << "Can exclude " << toExclude.size() << " nodes." << std::endl;
  return toExclude;
}

int main (int argc, char **argv)
{
  int useLB = 0;
  if (argc == 2) {
      useLB = std::stoi(std::string(argv[1]));
  }

  int vertexCount;
  int edgeCount;
  std::cin >> vertexCount >> edgeCount;

  Graph graph = parseIncidenceMatrix(vertexCount, edgeCount);
  std::cout << "Parsed" << std::endl;
  std::set<int> nonUsefulNodes = findNonUsefulNodes(vertexCount, graph);
  // Result result = nilcatenationCplex(graph, vertexCount, 600, useLB == 1, nonUsefulNodes);

  // std::cout
  //   << result.objectiveValue << " "
  //   << result.gap << " "
  //   << (result.isOptimal ? "1" : "0") << " "
  //   << (verifySolution(graph.incidenceMatrix, result, vertexCount) ? "1" : "0")
  //   << std::endl;

  // std::cout << std::endl;
}
