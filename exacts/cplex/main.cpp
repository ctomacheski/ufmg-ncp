#include <iostream>
#include <vector>
#include <chrono>

#include "cplex.h"

int main (int argc, char **argv)
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    
    int vertexCount;
    int edgeCount;
    std::cin >> vertexCount >> edgeCount;

    std::vector<Edge> incidenceMatrix = parseIncidenceMatrix(vertexCount, edgeCount);
    Result result = nilcatenationCplex(incidenceMatrix, vertexCount);

    std::cout << result.objectiveValue << " " << (result.isOptimal ? "1" : "0") << std::endl;

    // for (long i = 0; i < result.variablesResult.size(); i++) {
    //     std::cout << result.variablesResult[i];
    //     if (i != result.variablesResult.size() - 1) std::cout << " ";
    // }
    // std::cout << std::endl;

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
}
