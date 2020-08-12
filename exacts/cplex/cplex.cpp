#include <ilcplex/ilocplex.h>
#include <vector>

#include "cplex.h"

std::vector<Edge> parseIncidenceMatrix(int vertexCount, int edgeCount) {
    std::vector<Edge> incidenceMatrix(edgeCount);

    for (int i = 0; i < edgeCount; i++) {
        int from, to, weight;
        std::cin >> from >> to >> weight;

        incidenceMatrix[i].from = from;
        incidenceMatrix[i].to = to;
        incidenceMatrix[i].weight = weight;
    }

    return incidenceMatrix;
}


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

Result nilcatenationCplex(
    std::vector<Edge> incidenceMatrix,
    int nodeCount,
    int timeLimit
) {
    IloEnv env;

    try {
        IloModel model(env);

        IloBoolVarArray vars(env);
        IloExpr objective(env);
        for (int i = 0; i < incidenceMatrix.size(); i++) {
            vars.add(IloBoolVar(env));
            objective += vars[i]; 
        }

        model.add(objective > 0);
        model.add(IloMaximize(env, objective));

        IloExprArray constraints(env);
        for (int i = 0; i < nodeCount; i++) {
            constraints.add(IloExpr(env));
        }

        for (int i = 0; i < incidenceMatrix.size(); i++) {
            Edge e = incidenceMatrix[i];
            constraints[e.from] -= e.weight * vars[i];
            constraints[e.to] += e.weight * vars[i];
        }

        for (int i = 0; i < nodeCount; i++) {
            model.add(constraints[i] == 0);
        }

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());
        cplex.setParam(IloCplex::TiLim, timeLimit);

        if (!cplex.solve()) {
            return { false };
        }

        bool isInfeasible = cplex.getStatus() == IloAlgorithm::Infeasible;
        if (isInfeasible) {
            return { true, isInfeasible };
        }

        IloNumArray values(env);
        cplex.getValues(values, vars);

        std::vector<int> resultVariables(incidenceMatrix.size());
        for (long i = 0; i < incidenceMatrix.size(); i++) {
            resultVariables[i] = (values[i] > 0 ? 1 : 0);
        }

        bool isOptimal = cplex.getStatus() == IloAlgorithm::Optimal;

        return { true, isInfeasible, isOptimal, (int)cplex.getObjValue(), resultVariables };
    }
    catch (IloException& e) {
        std::cerr << "Concert exception caught: " << e << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown exception caught" << std::endl;
    }

    return { false };

    env.end();
}

Result nilcatenationCplex(
    std::vector<Edge> incidenceMatrix,
    int nodeCount,
    int timeLimit,
    int lowerBound,
    bool returnFirstSolution,
    std::vector<BranchingConstraint> branchingContraints
) {
    IloEnv env;

    try {
        IloModel model(env);

        IloBoolVarArray vars(env);
        IloExpr objective(env);
        for (int i = 0; i < incidenceMatrix.size(); i++) {
            vars.add(IloBoolVar(env));
            objective += vars[i]; 
        }

        model.add(objective > 0);
        model.add(IloMaximize(env, objective));

        for (BranchingConstraint localBranching : branchingContraints) {
            IloExpr inSolution(env);
            IloExpr notInSolution(env);

            for (int i = 0; i < localBranching.referenceSolution.size(); i++) {
                if (localBranching.referenceSolution[i] == 1) {
                    inSolution += (1 - vars[i]); 
                }
                else {
                    notInSolution += vars[i];
                }
            }

            if (localBranching.isLeft) {
                model.add(inSolution + notInSolution <= localBranching.rhs);
            }
            else {
                model.add(inSolution + notInSolution >= localBranching.rhs);
            }
        }

        IloExprArray constraints(env);
        for (int i = 0; i < nodeCount; i++) {
            constraints.add(IloExpr(env));
        }

        for (int i = 0; i < incidenceMatrix.size(); i++) {
            Edge e = incidenceMatrix[i];
            constraints[e.from] -= e.weight * vars[i];
            constraints[e.to] += e.weight * vars[i];
        }

        for (int i = 0; i < nodeCount; i++) {
            model.add(constraints[i] == 0);
        }

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());
        cplex.setParam(IloCplex::TiLim, timeLimit);
        cplex.setParam(IloCplex::Param::MIP::Tolerances::LowerCutoff, lowerBound + 1);

        if (returnFirstSolution) {
            cplex.setParam(IloCplex::Param::MIP::Limits::Solutions, 1);
        }

        if (!cplex.solve()) {
            return { false };
        }

        bool isInfeasible = cplex.getStatus() == IloAlgorithm::Infeasible;
        if (isInfeasible) {
            return { true, isInfeasible };
        }

        IloNumArray values(env);
        cplex.getValues(values, vars);

        std::vector<int> resultVariables(incidenceMatrix.size());
        for (long i = 0; i < incidenceMatrix.size(); i++) {
            resultVariables[i] = (values[i] > 0 ? 1 : 0);
        }

        bool isOptimal = cplex.getStatus() == IloAlgorithm::Optimal;

        return { true, isInfeasible, isOptimal, (int)cplex.getObjValue(), resultVariables };
    }
    catch (IloException& e) {
        std::cerr << "Concert exception caught: " << e << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown exception caught" << std::endl;
    }

    return { false };

    env.end();
}