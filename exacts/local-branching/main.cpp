#include <iostream>
#include <vector>
#include <chrono>
#include <limits>
#include <cmath>

#include "../cplex/cplex.h"

const int timeLimitNode = 60;
const int maxTimeLimit = 60 * 10;
const int initialK = 10;
const int maxDiversify = 50;
 
Result localBranching(int nodeCount, std::vector<Edge> incidenceMatrix) {
    std::cout << "Starting localBranching" << std::endl;

    int remainingTime = maxTimeLimit;
    int timeLimit = std::numeric_limits<int>::max();
    int k = initialK;

    bool returnFirstSolution = true;
    bool diversify = false;

    int bestLowerBound = 0;
    int lowerBound = 0;
    int diversifyCount = 0;

    int rhs = std::numeric_limits<int>::max();

    Result incubentSolution = { };
    Result referenceSolution = { };

    std::vector<BranchingConstraint> constraints;
    bool isRight = false;
    while (remainingTime > 0 && diversifyCount <= maxDiversify) {

        std::cout << std::endl << "incubentSolution value " << incubentSolution.objectiveValue << std::endl;

        if (rhs < std::numeric_limits<int>::max() && referenceSolution.variablesResult.size() > 0) {
            if (!isRight) {
                // adding left branching constraint (x, x') <= rhs
                std::cout << "Adding left branch constraint (x, x') <= " << rhs << std::endl;
                constraints.push_back({ referenceSolution.variablesResult, true, rhs });
            }
            else {
                isRight = false;
            }
        }

        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
     
        timeLimit = std::min(timeLimit, remainingTime);
        std::cout << "timeLimit " << timeLimit << std::endl;
         std::cout << "first " << (returnFirstSolution ? "yes" : "no") << std::endl;
        std::cout << "constraints size " << constraints.size() << std::endl;
        Result newSolution = nilcatenationCplex(incidenceMatrix, nodeCount, timeLimit, lowerBound, returnFirstSolution, constraints);

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        int elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
        remainingTime -= elapsedTime;
        timeLimit = timeLimitNode;

        if (newSolution.isSolutionFound && newSolution.isOptimal) {
            std::cout << "Found and Optimal" << std::endl;
            if (newSolution.objectiveValue > bestLowerBound) {
                bestLowerBound = newSolution.objectiveValue;
                incubentSolution = newSolution;
            } else {
                std::cout << "Not improving " << newSolution.objectiveValue << std::endl;
            }

            if (rhs == std::numeric_limits<int>::max()) {
                // optimal solution to original problem
                std::cout << "Optimal solution found" << std::endl;
                return incubentSolution;
            }

            if (constraints.size() > 0 && referenceSolution.variablesResult.size() > 0) {
                // reverse last branching constraint to (x, x') >= rhs + 1
                if (!constraints[constraints.size() - 1].isLeft) { 
                    std::cout << "SHOULD BE A LEFT BRANCH" << std::endl;
                } 
                std::cout << "Reverse last branching constraint to (x, x') >= " << rhs + 1 << std::endl;
                constraints[constraints.size() - 1] = { referenceSolution.variablesResult, false, rhs + 1 };
                isRight = true;
            }

            returnFirstSolution = false;
            diversify = false;
            referenceSolution = newSolution;
            lowerBound = newSolution.objectiveValue;
            rhs = k;
        } else if (newSolution.isSolutionFound && newSolution.isInfeasible) {
            std::cout << "Found and Infeasible" << std::endl;
            if (rhs == std::numeric_limits<int>::max()) {
                // original problem is infeasible
                std::cout << "Original problem is infeasible" << std::endl;
                return { false, true };
            }

            if (constraints.size() > 0  && referenceSolution.variablesResult.size() > 0) {
                // reverse last branching constraint to (x, x') >= rhs + 1
                if (!constraints[constraints.size() - 1].isLeft) { 
                    std::cout << "SHOULD BE A LEFT BRANCH" << std::endl;
                }
                std::cout << "Reverse last branching constraint to (x, x') >= " << rhs + 1 << std::endl;
                constraints[constraints.size() - 1] = { referenceSolution.variablesResult, false, rhs + 1 };
                isRight = true;
            }

            if (diversify) {
                timeLimit = std::numeric_limits<int>::max();
                lowerBound = 0;
                returnFirstSolution = true;
                diversifyCount++;
            }

            rhs += std::ceil((double)k/2);
            diversify = true;
        } else if (newSolution.isSolutionFound) {
            std::cout << "Found and Feasible" << std::endl;
            if (rhs < std::numeric_limits<int>::max()) {
                if (returnFirstSolution) {
                    if (constraints.size() > 0) {
                        // delete last left branch constraint
                        constraints.pop_back();
                    }
                }
                else { 
                    if (constraints.size() > 0 && referenceSolution.variablesResult.size() > 0) {
                        // reverse last branching constraint to (x, x') >= 1
                        if (!constraints[constraints.size() - 1].isLeft) { 
                            std::cout << "SHOULD BE A LEFT BRANCH" << std::endl;
                        }
                        else {
                            std::cout << "Reverse last branching constraint to (x, x') >= " << 1 << std::endl;
                            constraints[constraints.size() - 1] = { referenceSolution.variablesResult, false, 1 };
                        }
                    }
                }
            }

             if (newSolution.objectiveValue > bestLowerBound) {
                bestLowerBound = newSolution.objectiveValue;
                incubentSolution = newSolution;
            } else {
                std::cout << "Not improving " << newSolution.objectiveValue << std::endl;
            }

            returnFirstSolution = false;
            diversify = false;
            referenceSolution = newSolution;
            lowerBound = newSolution.objectiveValue;
            rhs = k;
        } else {
            std::cout << "Not found" << std::endl;
            if (diversify) {
                if (constraints.size() > 0 && referenceSolution.variablesResult.size() > 0) {
                    // reverse last branching constraint to (x, x') >= 1
                    if (!constraints[constraints.size() - 1].isLeft) { 
                        std::cout << "SHOULD BE A LEFT BRANCH" << std::endl;
                    }
                    else {
                        std::cout << "Reverse last branching constraint to (x, x') >= " << 1 << std::endl;
                        constraints[constraints.size() - 1] = { referenceSolution.variablesResult, false, 1 };
                    }
                }

                timeLimit = std::numeric_limits<int>::max();
                rhs += std::ceil((double)k/2);
                lowerBound = 0;
                returnFirstSolution = true;
                diversifyCount++;
            } else {
                if (constraints.size() > 0) {
                    // delete last left branch constraint
                    constraints.pop_back();
                }
                rhs -= std::ceil((double)k/2);
            }
            diversify = true;
        }
    }

    std::cout << std::endl << "End round" << std::endl;

    timeLimit = std::max(remainingTime, 0);
    std::cout << "constraints size " << constraints.size() << std::endl;
    Result finalSolution = nilcatenationCplex(incidenceMatrix, nodeCount, timeLimit, bestLowerBound, false, constraints);

    if (finalSolution.isSolutionFound && finalSolution.objectiveValue > incubentSolution.objectiveValue) {
        std::cout << "Found a better solution" << std::endl;
        return finalSolution;
    }

    return incubentSolution;
}

int main (int argc, char **argv)
{
    int nodeCount;
    int edgeCount;
    std::cin >> nodeCount >> edgeCount;

    std::vector<Edge> incidenceMatrix = parseIncidenceMatrix(nodeCount, edgeCount);
    Result result = localBranching(nodeCount, incidenceMatrix);

    std::cout
        << result.objectiveValue << " "
        << (result.isOptimal ? "1" : "0") << " "
        << (verifySolution(incidenceMatrix, result, nodeCount) ? "1" : "0")
        << std::endl;
}
