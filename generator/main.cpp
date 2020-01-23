#include <iostream>
#include <vector>
#include <random>

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "Wrong usage." << std::endl;
    return -1;
  }

  int nodeSize = std::stoi(std::string(argv[1]));
  double density = std::stod(std::string(argv[2]));

  std::vector<int> discreteWeights = { 1, 2, 5, 7, 10 };

  int edgesCount = 0;
  std::vector<std::vector<std::pair<int, int> > > graph(nodeSize, std::vector<std::pair<int, int> >());
  for (int i = 0; i < nodeSize; i++) {
    for (int j = 0; j < nodeSize; j++) {
      if (i != j) {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> distInt(0, discreteWeights.size()-1);
        std::uniform_int_distribution<std::mt19937::result_type> distMultipleEdges(1, 3);
        std::uniform_real_distribution<> distDouble(0, 1);
        std::uniform_real_distribution<> distDoubleMultiple(0, 1);


        double coinEdge = distDouble(rng);
        double coinMultipleEdgesDouble = distDouble(rng);

        bool hasEdge = coinEdge <= density;
        if (hasEdge) {
          int coinMultipleEdges = coinMultipleEdges <= 0.05 ? distMultipleEdges(rng) : 1;
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

  std::cout << nodeSize << " " << edgesCount << std::endl;
  for (int i = 0; i < nodeSize; i++) {
    for (int j = 0; j < graph[i].size(); j++) {
      std::cout << i << " " << graph[i][j].first << " " << graph[i][j].second << std::endl;
    }
  }

  return 0;
}