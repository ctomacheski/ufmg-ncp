#ifndef CPLEX_H
#define CPLEX_H

#include <vector>

struct Result {
    bool isSolutionFound;
    bool isInfeasible;
    bool isOptimal;
    int objectiveValue;
    std::vector<int> variablesResult;
};

struct BranchingConstraint {
    std::vector<int> referenceSolution;
    bool isLeft;
    int rhs;
};

struct Edge {
    int from;
    int to;
    long weight;
};

std::vector<Edge> parseIncidenceMatrix(int vertexCount, int edgeCount);

bool verifySolution(std::vector<Edge> incidenceMatrix, Result result, int nodeCount);

Result nilcatenationCplex(
    std::vector<Edge> incidenceMatrix,
    int nodeCount,
    int timeLimit,
    int lowerBound,
    bool returnFirstSolution,
    std::vector<BranchingConstraint> branchingContraints
);

Result nilcatenationCplex(
    std::vector<Edge> incidenceMatrix,
    int nodeCount,
    int timeLimit,
    bool useLB
);

#endif