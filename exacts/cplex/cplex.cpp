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

Result nilcatenationCplex(
    bool localBranch,
    bool isLeft,
    std::vector<int> initialSolution,
    std::vector<Edge> incidenceMatrix,
    int nodeCount
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
        model.add(IloMaximize(env, objective));

        if (localBranch) {
            IloExpr inSolution(env);
            IloExpr notInSolution(env);

            for (int i = 0; i < initialSolution.size(); i++) {
                if (initialSolution[i] == 1) {
                    inSolution += (1 - vars[i]); 
                }
                else {
                    notInSolution += vars[i];
                }
            }

            if (isLeft) {
                model.add(inSolution + notInSolution <= 1);
            }
            else {
                model.add(inSolution + notInSolution >= 11);
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
        //cplex.setOut(env.getNullStream());
        cplex.setParam(IloCplex::TiLim, localBranch ? 600 : 30);
        cplex.setParam(IloCplex::Param::Emphasis::MIP, 1);

        if (!cplex.solve()) {
            return { false };
        }

        IloNumArray values(env);
        cplex.getValues(values, vars);

        std::vector<int> resultVariables(incidenceMatrix.size());
        for (long i = 0; i < incidenceMatrix.size(); i++) {
            resultVariables[i] = (values[i] > 0 ? 1 : 0);
        }

        bool isOptimal = cplex.getStatus() == IloAlgorithm::Optimal;

        return { true, isOptimal, (int)cplex.getObjValue(), resultVariables };
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

Result nilcatenationCplex(std::vector<Edge> incidenceMatrix, int nodeCount) {
    return nilcatenationCplex(false, false, std::vector<int>(), incidenceMatrix, nodeCount);
}