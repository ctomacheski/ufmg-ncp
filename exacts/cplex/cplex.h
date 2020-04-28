#ifndef CPLEX_H
#define CPLEX_H

#include <vector>

struct Result {
    bool foundSolution;
    bool isOptimal;
    int objectiveValue;
    std::vector<int> variablesResult;
};

struct Edge {
    int from;
    int to;
    int weight;
};

std::vector<Edge> parseIncidenceMatrix(int vertexCount, int edgeCount);

Result nilcatenationCplex(
    std::vector<Edge> incidenceMatrix,
    int nodeCount
);

Result nilcatenationCplex(
    bool localBranch,
    bool isLeft,
    std::vector<int> initialSolution,
    std::vector<Edge> incidenceMatrix,
    int nodeCount
);

#endif