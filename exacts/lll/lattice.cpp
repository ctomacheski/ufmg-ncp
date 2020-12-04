#include <iostream>
#include <vector>
#include <cmath>

bool verifySolution(std::vector<std::vector<double>> incidenceMatrix, std::vector<int> solution, int n, int m)
{
  int balance = 0;
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < m; j++)
    {
      if (solution[j] == 1)
      {
        balance += incidenceMatrix[i][j];
      }
    }
  }
  return balance == 0;
}

std::vector<std::vector<double>> parseIncidenceMatrix(int vertexCount, int edgeCount)
{
  std::vector<std::vector<double>> incidenceMatrix(vertexCount, std::vector<double>(edgeCount));

  for (int i = 0; i < edgeCount; i++)
  {
    int from, to;
    double weight;
    std::cin >> from >> to >> weight;

    incidenceMatrix[from][i] = -weight;
    incidenceMatrix[to][i] = weight;
  }

  return incidenceMatrix;
}

void printLattice(std::vector<std::vector<double>> basis, int n, int m)
{
  std::cout << "[";
  for (int i = 0; i < n; i++)
  {
    std::cout << "[";
    for (int j = 0; j < m; j++)
    {
      std::cout << basis[i][j];
      if (j != m - 1)
        std::cout << " ";
    }

    std::cout << "]";
    if (i != n - 1)
      std::cout << std::endl;
  }
  std::cout << "]";
  std::cout << std::endl;
}

int main(int argc, char *argv[])
{
  int n_graph;
  int m_graph;
  std::cin >> n_graph >> m_graph;

  std::vector<std::vector<double>> incidenceMatrix = parseIncidenceMatrix(n_graph, m_graph);

  std::vector<std::vector<double>> lattice = std::vector<std::vector<double>>(m_graph, std::vector<double>(m_graph + n_graph));

  for (int i = 0; i < m_graph; i++)
  {
    lattice[i][i] = 1;

    // create coeficients
    for (int j = 0; j < n_graph; j++)
    {
      lattice[i][j + m_graph] = incidenceMatrix[j][i];
    }
  }

  printLattice(lattice, m_graph, m_graph + n_graph);

  return 0;
}
