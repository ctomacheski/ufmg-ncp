#ifndef CPLEX_H
#define CPLEX_H

#include <vector>

#include "../../util/types.h"

Graph parseIncidenceMatrix(int vertexCount, int edgeCount);

bool verifySolution(std::vector<Edge> incidenceMatrix, Result result, int nodeCount);

Result nilcatenationCplex(
    std::vector<Edge> incidenceMatrix,
    int nodeCount,
    int timeLimit,
    int lowerBound,
    bool returnFirstSolution,
    int solutionCount,
    std::vector<BranchingConstraint> branchingContraints);

Result nilcatenationCplex(
    Graph graph,
    int nodeCount,
    int timeLimit,
    bool useLB);

#endif