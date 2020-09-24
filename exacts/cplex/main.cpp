#include <iostream>
#include <vector>

#include "cplex.h"

int main (int argc, char **argv)
{
    int useLB = 0;
    if (argc == 2) {
        useLB = std::stoi(std::string(argv[1]));
    }

    int vertexCount;
    int edgeCount;
    std::cin >> vertexCount >> edgeCount;

    std::vector<Edge> incidenceMatrix = parseIncidenceMatrix(vertexCount, edgeCount);
    Result result = nilcatenationCplex(incidenceMatrix, vertexCount, 3000, useLB == 1);

    std::cout
        << result.objectiveValue << " "
        << (result.isOptimal ? "1" : "0") << " "
        << (verifySolution(incidenceMatrix, result, vertexCount) ? "1" : "0")
        << std::endl;

    for (int i = 0; i < result.variablesResult.size(); i++) {
        std::cout << result.variablesResult[i] << " ";
    }
    std::cout << std::endl;
}
