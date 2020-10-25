#include <iostream>
#include <vector>
#include <random>

#include "../util/kosaraju.h"

int main(int argc, char *argv[]) {
  if (argc < 4) {
    std::cerr << "Wrong usage." << std::endl;
    return -1;
  }

  std::random_device dev;
  std::mt19937 rng(dev());

  int nodeSize = std::stoi(std::string(argv[1]));
  double density = std::stod(std::string(argv[2]));
  double dintictionProb = std::stod(std::string(argv[3]));

  int weightsSize = nodeSize * dintictionProb;
  std::vector<int> discreteWeights(weightsSize);
  std::uniform_int_distribution<std::mt19937::result_type> distWeights(1, weightsSize * 10);

  for (int i = 0; i < weightsSize; i++) {
    int weight = distWeights(rng);
    discreteWeights[i] = weight;
  }

  int edgesCount = 0;
  std::vector<std::vector<std::pair<int, int> > > graph;
  bool hasOnlyOneScc = false;

  while (!hasOnlyOneScc) {
    edgesCount = 0;
    graph = std::vector<std::vector<std::pair<int, int> > >(nodeSize, std::vector<std::pair<int, int> >());
    for (int i = 0; i < nodeSize; i++) {
      for (int j = 0; j < nodeSize; j++) {
        if (i != j) {
          std::uniform_int_distribution<std::mt19937::result_type> distInt(0, discreteWeights.size()-1);
          std::uniform_int_distribution<std::mt19937::result_type> distMultipleEdges(1, 3);
          std::uniform_real_distribution<> distDouble(0, 1);
          std::uniform_real_distribution<> distDoubleMultiple(0, 1);

          double coinEdge = distDouble(rng);
          double coinMultipleEdgesDouble = distDouble(rng);

          bool hasEdge = coinEdge <= density;
          if (hasEdge) {
            int coinMultipleEdges = coinMultipleEdgesDouble <= 0.05 ? distMultipleEdges(rng) : 1;
            for (int k = 0; k < coinMultipleEdges; k++) {
              int coinWeight = distInt(rng);
              int weight = discreteWeights[coinWeight];
              edgesCount++;
              graph[i].push_back(std::make_pair(j, weight));
            }
          }
        }
      }
    }

    std::map<int, std::set<int> > components = kosarajuAlgorithm(graph);
    hasOnlyOneScc = 1 == components.size();
  }

  std::cout << nodeSize << " " << edgesCount << std::endl;
  for (int i = 0; i < nodeSize; i++) {
    for (int j = 0; j < graph[i].size(); j++) {
      std::cout << i << " " << graph[i][j].first << " " << graph[i][j].second << std::endl;
    }
  }

  return 0;
}