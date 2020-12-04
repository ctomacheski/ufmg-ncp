#include <iostream>
#include <vector>
#include <chrono>
#include <limits>
#include <cmath>

#include "../cplex/cplex.h"
#include "../../util/pre-processing.h"

Result localBranching(
    int nodeCount,
    std::vector<Edge> incidenceMatrix,
    int timeLimitNode,
    int maxTimeLimit,
    int initialK,
    int maxDiversify)
{
    int remainingTime = maxTimeLimit;
    int timeLimit = std::numeric_limits<int>::max();
    int k = initialK;

    bool returnFirstSolution = true;
    bool diversify = false;

    int bestLowerBound = 0;
    int lowerBound = 0;
    int diversifyCount = 0;

    int rhs = std::numeric_limits<int>::max();

    Result incubentSolution = {};
    Result referenceSolution = {};

    std::vector<BranchingConstraint> constraints;
    bool isRight = false;
    bool isRoot = true;
    int counter = 0;

    while (remainingTime > 0 && diversifyCount <= maxDiversify)
    {

        if (rhs < std::numeric_limits<int>::max() && referenceSolution.variablesResult.size() > 0)
        {
            if (!isRight)
            {
                constraints.push_back({referenceSolution.variablesResult, true, rhs});
            }
            else
            {
                isRight = false;
            }
        }

        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        timeLimit = std::min(timeLimit, remainingTime);
        counter = counter + 1;

        Result newSolution = nilcatenationCplex(incidenceMatrix, nodeCount, timeLimit, lowerBound, returnFirstSolution, 3, constraints);

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        int elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
        remainingTime -= elapsedTime;
        timeLimit = timeLimitNode;

        if (newSolution.isSolutionFound && newSolution.isOptimal)
        {

            if (newSolution.objectiveValue > bestLowerBound)
            {
                bestLowerBound = newSolution.objectiveValue;
                incubentSolution = newSolution;
            }

            if (rhs == std::numeric_limits<int>::max())
            {
                return incubentSolution;
            }

            if (constraints.size() > 0 && referenceSolution.variablesResult.size() > 0)
            {
                constraints[constraints.size() - 1] = {referenceSolution.variablesResult, false, rhs + 1};
                isRight = true;
            }

            returnFirstSolution = false;
            diversify = false;
            referenceSolution = newSolution;
            lowerBound = newSolution.objectiveValue;
            rhs = k;
        }
        else if (newSolution.isSolutionFound && newSolution.isInfeasible)
        {
            if (rhs == std::numeric_limits<int>::max())
            {
                return {false, true};
            }

            if (constraints.size() > 0 && referenceSolution.variablesResult.size() > 0)
            {
                constraints[constraints.size() - 1] = {referenceSolution.variablesResult, false, rhs + 1};
                isRight = true;
            }

            if (diversify)
            {
                timeLimit = std::numeric_limits<int>::max();
                lowerBound = 0;
                returnFirstSolution = true;
                diversifyCount++;
            }

            rhs += std::ceil((double)k / 2);
            diversify = true;
        }
        else if (newSolution.isSolutionFound)
        {
            if (rhs < std::numeric_limits<int>::max())
            {
                if (returnFirstSolution)
                {
                    if (constraints.size() > 0)
                    {
                        constraints.pop_back();
                    }
                }
                else
                {
                    if (constraints.size() > 0 && referenceSolution.variablesResult.size() > 0)
                    {
                        if (constraints[constraints.size() - 1].isLeft)
                        {
                            constraints[constraints.size() - 1] = {referenceSolution.variablesResult, false, 1};
                        }
                    }
                }
            }

            if (newSolution.objectiveValue > bestLowerBound)
            {
                bestLowerBound = newSolution.objectiveValue;
                incubentSolution = newSolution;
            }

            returnFirstSolution = false;
            diversify = false;
            referenceSolution = newSolution;
            lowerBound = newSolution.objectiveValue;
            rhs = k;
        }
        else
        {
            if (diversify)
            {
                if (constraints.size() > 0 && referenceSolution.variablesResult.size() > 0)
                {
                    if (constraints[constraints.size() - 1].isLeft)
                    {
                        constraints[constraints.size() - 1] = {referenceSolution.variablesResult, false, 1};
                    }
                }

                timeLimit = std::numeric_limits<int>::max();
                rhs += std::ceil((double)k / 2);
                lowerBound = 0;
                returnFirstSolution = true;
                diversifyCount++;
            }
            else
            {
                if (constraints.size() > 0)
                {
                    constraints.pop_back();
                }
                rhs -= std::ceil((double)k / 2);
            }
            diversify = true;
        }
    }

    timeLimit = std::max(remainingTime, timeLimitNode);
    Result finalSolution = nilcatenationCplex(incidenceMatrix, nodeCount, timeLimit, bestLowerBound, false, 1, constraints);

    if (finalSolution.isSolutionFound && finalSolution.objectiveValue >= incubentSolution.objectiveValue)
    {
        return finalSolution;
    }

    return {incubentSolution.isSolutionFound,
            incubentSolution.isInfeasible,
            false,
            incubentSolution.objectiveValue,
            incubentSolution.variablesResult};
}

int main(int argc, char **argv)
{
    int nodeCount;
    int edgeCount;
    std::cin >> nodeCount >> edgeCount;

    int maxTimeLimit = 3600;
    const int timeLimitNode = 300;
    maxTimeLimit -= timeLimitNode;

    const int initialK = 5;
    const int maxDiversify = std::numeric_limits<int>::max(); //nodeCount * 100;

    Graph graph = parseIncidenceMatrix(nodeCount, edgeCount);
    preProcess(graph);
    Result result = localBranching(nodeCount, graph.incidenceMatrix, timeLimitNode, maxTimeLimit, initialK, maxDiversify);

    std::cout
        << result.objectiveValue << " "
        << result.gap << " "
        << (result.isOptimal ? "1" : "0") << " "
        << (verifySolution(graph.incidenceMatrix, result, nodeCount) ? "1" : "0")
        << std::endl;
}
