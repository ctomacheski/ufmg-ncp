#include <iostream>
#include <vector>
#include <chrono>

#include "../cplex/cplex.h"

bool verifySolution(std::vector<Edge> incidenceMatrix, Result result, int nodeCount) {
    std::vector<int> graphBalance(nodeCount);

    for (int i = 0; i < result.variablesResult.size(); i++) {
        if (result.variablesResult[i] == 1) {
            graphBalance[incidenceMatrix[i].from] -= incidenceMatrix[i].weight;
            graphBalance[incidenceMatrix[i].to] += incidenceMatrix[i].weight;
        }
    }

    bool isCorrect = true;
    for (int i = 0; i < graphBalance.size(); i++) {
        isCorrect &= graphBalance[i] == 0;
    }

    return isCorrect;
}

int main (int argc, char **argv)
{
    int nodeCount;
    int edgeCount;
    std::cin >> nodeCount >> edgeCount;

    std::vector<Edge> incidenceMatrix = parseIncidenceMatrix(nodeCount, edgeCount);
    Result bestSolution = nilcatenationCplex(incidenceMatrix, nodeCount);

    std::cout
        << "initial "
        << bestSolution.objectiveValue
        << " "
        << (bestSolution.isOptimal ? "1" : "0")
        << " "
        << (verifySolution(incidenceMatrix, bestSolution, nodeCount) ? "1" : "0")
        << std::endl;

    long elapsedTime = 0;
    bool isImproving = true;
    while (elapsedTime < 600 && isImproving) {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        Result branchResultLeft = nilcatenationCplex(true, true, bestSolution.variablesResult, incidenceMatrix, nodeCount);
        std::cout
            << "final "
            << branchResultLeft.objectiveValue
            << " "
            << (branchResultLeft.isOptimal ? "1" : "0")
            << " "
            << (verifySolution(incidenceMatrix, branchResultLeft, nodeCount) ? "1" : "0")
            << std::endl;

        isImproving = false;
        // Result branchResultRight = nilcatenationCplex(true, false, bestSolution.variablesResult, incidenceMatrix, nodeCount);

        // std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        // elapsedTime += std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();

        // isImproving = false;
        // if (branchResultLeft.foundSolution || branchResultRight.foundSolution) {
        //     std::cout
        //         << "found"
        //         << std::endl;

        //     Result bestBranch = branchResultLeft.foundSolution && branchResultRight.foundSolution
        //         ? (branchResultLeft.objectiveValue > branchResultRight.objectiveValue ? branchResultLeft : branchResultRight)
        //         : (branchResultLeft.foundSolution ? branchResultLeft : branchResultRight);
            
        //     if (bestBranch.objectiveValue > bestSolution.objectiveValue) {
        //         bestSolution = bestBranch;
        //         isImproving = true;

        //         std::cout
        //             << "branch "
        //             << bestSolution.objectiveValue
        //             << " "
        //             << (bestSolution.isOptimal ? "1" : "0")
        //             << " "
        //             << (verifySolution(incidenceMatrix, bestSolution, nodeCount) ? "1" : "0")
        //             << std::endl;
        //     }
        // } 
    }

    std::cout
        << "final "
        << bestSolution.objectiveValue
        << " "
        << (bestSolution.isOptimal ? "1" : "0")
        << " "
        << (verifySolution(incidenceMatrix, bestSolution, nodeCount) ? "1" : "0")
        << std::endl;
}
