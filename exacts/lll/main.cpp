#include <iostream>
#include <vector>
#include <cmath>

bool verifySolution(std::vector<std::vector<double> > incidenceMatrix, std::vector<int> solution, int n, int m) {
  int balance = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (solution[j] == 1) {
        balance += incidenceMatrix[i][j];
      }
    }
  }
  return balance == 0;
}

std::vector<std::vector<double> > parseIncidenceMatrix(int vertexCount, int edgeCount) {
    std::vector<std::vector<double> > incidenceMatrix(vertexCount, std::vector<double>(edgeCount));

    double maxWeight = 0;
    for (int i = 0; i < edgeCount; i++) {
        int from, to;
        double weight;
        std::cin >> from >> to >> weight;

        maxWeight = std::max(maxWeight, std::abs(weight));

        incidenceMatrix[from][i] = -weight;
        incidenceMatrix[to][i] = weight;
    }

    double density = ((double)edgeCount)/(vertexCount * std::log2(maxWeight));

    std::cout << "density " << density << std::endl;

    return incidenceMatrix;
}

long calcU(std::vector<double> over, std::vector<double> below, int m) {
  long dotOver = 0;
  long dotBelow = 0;
  for (int i = 0; i < m; i++) {
    dotOver += over[i] * below[i];
    dotBelow += below[i] * below[i];
  }

  return std::lround((double)dotOver/dotBelow);
}

double calcUDouble(std::vector<double> over, std::vector<double> below, int m) {
  double dotOver = 0;
  double dotBelow = 0;
  for (int i = 0; i < m; i++) {
    dotOver += over[i] * below[i];
    dotBelow += below[i] * below[i];
  }

  return dotOver/dotBelow;
}

double norm(std::vector<double> v, int m) {
  double sum = 0;
  for (int i = 0; i < m; i++) {
    sum += v[i] * v[i];
  }

  return std::sqrt(sum);
}

void reductBasis(
  std::vector<std::vector<double> > &reducedBasis,
  std::vector<std::vector<double> > &orthogonalBasis,
  int n, int m
) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < i; j++) {
      long u = calcU(reducedBasis[i], orthogonalBasis[j], m);
      for (int k = 0; k < m; k++) {
        reducedBasis[i][k] -= u * reducedBasis[j][k];
      }
    }
  }
}

void findOrthogonalBasis(
  std::vector<std::vector<double> > &basis,
  std::vector<std::vector<double> > &orthogonalBasis,
  int n, int m
) {
  for (int i = 0; i < n; i++) {
    orthogonalBasis[i] = basis[i];
    for (int j = 0; j < i; j++) {
      double u = calcUDouble(basis[i], orthogonalBasis[j], m);
      for (int k = 0; k < m; k++) {
        orthogonalBasis[i][k] -= u * orthogonalBasis[j][k];
      }
    }
  }
}

void printBasis(std::vector<std::vector<double> > basis, int n, int m) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      std::cout << basis[i][j] << " ";
    }

    std::cout << std::endl;
  }
}

bool essentiallyEqual(float a, float b, float epsilon) {
  return fabs(a - b) <= ((fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

std::vector<std::vector<double> > lll(
  std::vector<std::vector<double> > &reducedBasis,
  std::vector<std::vector<double> > &orthogonalBasis,
  int n, int m
) {
  bool continueLoop = true;
  while (continueLoop) {
    continueLoop = false;
    findOrthogonalBasis(reducedBasis, orthogonalBasis, n, m);
    reductBasis(reducedBasis, orthogonalBasis, n, m);

    for (int i = 0; i < n - 1; i++) {
      double u = calcUDouble(reducedBasis[i+1], orthogonalBasis[i], m);
      std::vector<double> toCompare = std::vector<double>(m);
      for (int j = 0; j < m; j++) {
        toCompare[j] = orthogonalBasis[i+1][j] + u * orthogonalBasis[i][j];
      }

      double lhs = norm(toCompare, m);
      double rhs = 0.99 * norm(orthogonalBasis[i], m);
      if (lhs < rhs) {
        std::iter_swap(reducedBasis.begin() + i, reducedBasis.begin() + i+1);
        continueLoop = true;
        break;
      }
    }
  }

  return reducedBasis;
}

int main(int argc, char *argv[]) {
  //freopen("r_15_25_10.input","r", stdin);

  int n_graph;
  int m_graph;
  std::cin >> n_graph >> m_graph;


  std::vector<std::vector<double> > incidenceMatrix = parseIncidenceMatrix(n_graph, m_graph);

  std::vector<std::vector<double> > lattice = std::vector<std::vector<double> >(m_graph + 1, std::vector<double>(m_graph + n_graph + 1));

  int N_constant = std::lround(std::sqrt(((double)(m_graph+1))/4));

  for (int i = 0; i < m_graph; i++) {
    lattice[i][i] = 2;

    // create coeficients
    for (int j = 0; j < n_graph; j++) {
      lattice[i][j + m_graph] = incidenceMatrix[j][i] * N_constant;
    }
  }

  for (int i = 0; i < m_graph; i++) {
    lattice[m_graph][i] = 1;
  }

  lattice[m_graph][m_graph + n_graph] = 1;

  std::vector<std::vector<double> > orthogonalBasis = std::vector<std::vector<double> >(m_graph + 1, std::vector<double>(m_graph + n_graph + 1));
  std::vector<std::vector<double> > reducedBasis = lll(lattice, orthogonalBasis, m_graph + 1, m_graph + n_graph + 1);
  printBasis(reducedBasis, m_graph + 1, m_graph + n_graph + 1);

  std::vector<std::vector<int> > possibleSolutions;
  for (int i = 0; i < m_graph; i++) {
    std::vector<double> basisVector = reducedBasis[i];
    if (
      essentiallyEqual(std::abs(basisVector[m_graph + n_graph]), 1, 0.001) && 
      essentiallyEqual(std::abs(basisVector[m_graph + n_graph - 1]), 0, 0.001)
    ) {
      bool isSolution = true;
      for (int j = 0; j < m_graph; j++) {
        isSolution &= essentiallyEqual(std::abs(basisVector[j]), 1, 0.001);
      }
      if (isSolution) {
        std::vector<int> solution = std::vector<int>(m_graph);
        for (int j = 0; j < m_graph; j++) {
          solution[j] = std::abs(basisVector[j] - basisVector[m_graph + n_graph])/2;
        }
        possibleSolutions.push_back(solution);
      }
    }
  }

  for (int i = 0; i < possibleSolutions.size(); i++) {
    std::vector<int> solution = possibleSolutions[i];
    int obj = 0;
    for (int j = 0; j < m_graph; j++) {
      obj += solution[j];
    }
    bool isValid = verifySolution(incidenceMatrix, solution, n_graph, m_graph);
    std::cout << obj << " 0 " << (isValid ? "1" : "0") << std::endl;
    for (int j = 0; j < m_graph; j++) {
      std::cout << solution[j] << " ";
    }
    std::cout << std::endl;
  }

  return 0;
}
