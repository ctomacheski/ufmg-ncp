#ifndef CPLEX_H
#define CPLEX_H

#include <vector>

struct Result {
    bool isOptimal;
    int objectiveValue;
    std::vector<int> variablesResult;
};

Result nilcatenationCplex(
    std::vector<std::vector<int> > incidenceMatrix
);

Result nilcatenationCplex(
    bool localBranch,
    bool isLeft,
    std::vector<int> initialSolution,
    std::vector<std::vector<int> > incidenceMatrix
);

#endif