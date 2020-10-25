#ifndef CPLEX_H
#define CPLEX_H

#include <vector>
#include <set>

struct Result {
    bool isSolutionFound;
    bool isInfeasible;
    bool isOptimal;
    int objectiveValue;
    std::vector<int> variablesResult;
    double gap;
};

struct BranchingConstraint {
    std::vector<int> referenceSolution;
    bool isLeft;
    int rhs;
};

struct Edge {
    int id;
    int from;
    int to;
    long weight;
    const bool operator<(const Edge& a) const {
        return weight < a.weight;
    }
};

struct Graph {
    std::vector<Edge> incidenceMatrix;

    std::vector<std::set<Edge>> ingoingEdges;
    std::vector<std::set<Edge>> outgoingEdges;

    std::vector<int> ingoingBalance;
    std::vector<int> outgoingBalance;
};

Graph parseIncidenceMatrix(int vertexCount, int edgeCount);

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
    Graph graph,
    int nodeCount,
    int timeLimit,
    bool useLB
);

Result nilcatenationCplex(
    Graph graph,
    int nodeCount,
    int timeLimit,
    bool useLB,
    std::set<int> nonUsefulNodes
);

#endif