#include <iostream>
#include <vector>
#include <chrono>

#include "../cplex/cplex.h"

std::vector<std::vector<int> > parseIncidenceMatrix(int vertexCount, int edgeCount) {
    std::vector<std::vector<int> > incidenceMatrix(edgeCount, std::vector<int>(vertexCount));

    for (int i = 0; i < edgeCount; i++) {
        int from, to, weight;
        std::cin >> from >> to >> weight;

        incidenceMatrix[i][from] = -weight;
        incidenceMatrix[i][to] = weight;
    }

    return incidenceMatrix;
}

int main (int argc, char **argv)
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    
    int vertexCount;
    int edgeCount;
    std::cin >> vertexCount >> edgeCount;

    std::vector<std::vector<int> > incidenceMatrix = parseIncidenceMatrix(vertexCount, edgeCount);
    Result initialSolution = nilcatenationCplex(incidenceMatrix);
    std::cout << initialSolution.objectiveValue << " " << (initialSolution.isOptimal ? "1" : "0") << std::endl;

    bool teste = true;
    while (teste) {
        // std::cout << "left" << std::endl;
        // Result branchResultLeft = nilcatenationCplex(true, true, initialSolution.variablesResult, incidenceMatrix);
        // std::cout << branchResultLeft.objectiveValue << " " << (branchResultLeft.isOptimal ? "1" : "0") << std::endl;

        std::cout << "right" << std::endl;
        Result branchResultRight = nilcatenationCplex(true, false, initialSolution.variablesResult, incidenceMatrix);
        std::cout << branchResultRight.objectiveValue << " " << (branchResultRight.isOptimal ? "1" : "0") << std::endl;

        teste = false;
    }
    
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
}
