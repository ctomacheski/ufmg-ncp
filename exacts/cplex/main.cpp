#include <iostream>
#include <vector>

#include "cplex.h"

int main (int argc, char **argv)
{
    int vertexCount;
    int edgeCount;
    std::cin >> vertexCount >> edgeCount;

    std::vector<Edge> incidenceMatrix = parseIncidenceMatrix(vertexCount, edgeCount);
    Result result = nilcatenationCplex(incidenceMatrix, vertexCount, 360);

    std::cout
        << result.objectiveValue << " "
        << (result.isOptimal ? "1" : "0") << " "
        << (verifySolution(incidenceMatrix, result, vertexCount) ? "1" : "0")
        << std::endl;
}
