#include <iostream>
#include <vector>
#include <cmath>

std::vector<std::vector<double>> parseIncidenceMatrix(int vertexCount, int edgeCount)
{
  std::vector<std::vector<double>> incidenceMatrix(vertexCount, std::vector<double>(edgeCount));

  double sumWeight = 0;
  double allMaxWeight = 0;

  std::vector<double> maxWeight(vertexCount);
  std::vector<int> degrees(vertexCount);

  for (int i = 0; i < edgeCount; i++)
  {
    int from, to;
    double weight;
    std::cin >> from >> to >> weight;

    sumWeight += weight;
    allMaxWeight = std::max(allMaxWeight, weight);

    maxWeight[from] = std::max(maxWeight[from], weight);
    maxWeight[to] = std::max(maxWeight[to], weight);

    degrees[from]++;
    degrees[to]++;

    incidenceMatrix[from][i] = -weight;
    incidenceMatrix[to][i] = weight;
  }

  double density = ((double)edgeCount) / (vertexCount * std::log2(allMaxWeight));

  std::cout << "v " << vertexCount << std::endl;
  std::cout << "e " << edgeCount << std::endl;
  std::cout << "avg degree " << (double)edgeCount / vertexCount << std::endl;
  std::cout << "max weight " << allMaxWeight << std::endl;
  std::cout << "avg weight " << sumWeight / edgeCount << std::endl;
  std::cout << "density " << density << std::endl;

  double densitySum = 0;
  double maxDensity = 0;
  for (int i = 0; i < vertexCount; i++)
  {
    double densityNode = ((double)degrees[i]) / std::log2(maxWeight[i]);
    densitySum += densityNode;
    maxDensity = std::max(densityNode, maxDensity);
  }

  std::cout << "avg density " << densitySum / vertexCount << std::endl;
  std::cout << "max density " << maxDensity << std::endl;

  return incidenceMatrix;
}

int main()
{
  int n;
  int m;
  std::cin >> n >> m;

  parseIncidenceMatrix(n, m);

  return 0;
}