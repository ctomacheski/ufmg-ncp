#include <iostream>
#include <vector>
#include <cmath>

std::vector<std::vector<double> > parseIncidenceMatrix(int vertexCount, int edgeCount) {
  std::vector<std::vector<double> > incidenceMatrix(vertexCount, std::vector<double>(edgeCount));

  double maxWeight = 0;
  double sumWeight = 0;
  for (int i = 0; i < edgeCount; i++) {
    int from, to;
    double weight;
    std::cin >> from >> to >> weight;

    sumWeight += weight;
    maxWeight = std::max(maxWeight, weight);

    incidenceMatrix[from][i] = -weight;
    incidenceMatrix[to][i] = weight;
  }

  double density = ((double)edgeCount)/(vertexCount * std::log2(maxWeight));

  std::cout << "v " << vertexCount << std::endl;
  std::cout << "e " << edgeCount << std::endl;
  std::cout << "avg degree " << (double)edgeCount/vertexCount << std::endl;
  std::cout << "max weight " << maxWeight << std::endl;
  std::cout << "avg weight " << sumWeight/edgeCount << std::endl;
  std::cout << "density " << density << std::endl;

  return incidenceMatrix;
}

int main() {
  int n;
  int m;
  std::cin >> n >> m;
  
  parseIncidenceMatrix(n, m);

  return 0;
}