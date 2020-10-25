#include <ilcplex/ilocplex.h>
#include <vector>
#include <set>

#include "cplex.h"

Graph parseIncidenceMatrix(int vertexCount, int edgeCount) {
    std::vector<Edge> incidenceMatrix(edgeCount);

    std::vector<std::set<Edge>> ingoingEdges(vertexCount, std::set<Edge>());
    std::vector<std::set<Edge>> outgoingEdges(vertexCount, std::set<Edge>());

    std::vector<int> ingoingBalance(vertexCount);
    std::vector<int> outgoingBalance(vertexCount);

    for (int i = 0; i < edgeCount; i++) {
        int from, to;
        unsigned long weight;
        std::cin >> from >> to >> weight;

        incidenceMatrix[i].id = i;
        incidenceMatrix[i].from = from;
        incidenceMatrix[i].to = to;
        incidenceMatrix[i].weight = weight;

        ingoingBalance[to] += weight;
        ingoingEdges[to].insert(incidenceMatrix[i]);

        outgoingBalance[from] += weight;
        outgoingEdges[from].insert(incidenceMatrix[i]);
    }

    return { incidenceMatrix, ingoingEdges, outgoingEdges, ingoingBalance, outgoingBalance };
}


bool verifySolution(std::vector<Edge> incidenceMatrix, Result result, int nodeCount) {
    std::vector<unsigned long> graphBalance(nodeCount, 0);

    for (int i = 0; i < result.variablesResult.size(); i++) {
        graphBalance[incidenceMatrix[i].from] -= incidenceMatrix[i].weight * result.variablesResult[i];
        graphBalance[incidenceMatrix[i].to] += incidenceMatrix[i].weight * result.variablesResult[i];
    }

    bool isCorrect = true;
    for (int i = 0; i < graphBalance.size(); i++) {
        isCorrect &= graphBalance[i] == 0;
        if(graphBalance[i] != 0) std::cout << i << "," << graphBalance[i] << std::endl;;
    }

    return isCorrect;
}

bool essentiallyEqual(float a, float b, float epsilon)
{
    return fabs(a - b) <= ( (fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}


Result nilcatenationCplex(
    Graph graph,
    int nodeCount,
    int timeLimit,
    bool useLB,
    std::set<int> nonUsefulNodes
) {
    std::vector<Edge> incidenceMatrix = graph.incidenceMatrix;

    IloEnv env;

    try {
        IloModel model(env);

        IloIntVarArray vars(env);
        IloExpr objective(env);
        for (int i = 0; i < incidenceMatrix.size(); i++) {
            vars.add(IloIntVar(env, 0 , 1));
            objective += vars[i];
        }

        model.add(objective > 0);
        model.add(IloMaximize(env, objective));

        IloExprArray conditionNilc(env);
        for (int i = 0; i < nodeCount; i++) {
            conditionNilc.add(IloExpr(env));
        }

        for (int i = 0; i < incidenceMatrix.size(); i++) {
            Edge e = incidenceMatrix[i];
            conditionNilc[e.from] -= e.weight * vars[e.id];
            conditionNilc[e.to] += e.weight * vars[e.id];
        }

        for (int i = 0; i < nodeCount; i++) {
            model.add(conditionNilc[i] == 0);
        }

        IloCplex cplex(model);
        // cplex.setOut(env.getNullStream());
        cplex.setParam(IloCplex::TiLim, timeLimit);
        cplex.setParam(IloCplex::Param::RandomSeed, 1);

        if (useLB) {
            cplex.setParam(IloCplex::Param::MIP::Strategy::LBHeur, 1);
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
            resultVariables[i] = (essentiallyEqual(values[i], 1, 0.001) ? 1 : 0);
        }

        bool isOptimal = cplex.getStatus() == IloAlgorithm::Optimal;

        return { true, isInfeasible, isOptimal, (int)cplex.getObjValue(), resultVariables, (double)cplex.getMIPRelativeGap() };
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
        cplex.setParam(IloCplex::Param::RandomSeed, 1);

        if (returnFirstSolution) {
            cplex.setParam(IloCplex::Param::MIP::Limits::Solutions, 3);
            cplex.setParam(IloCplex::Param::Emphasis::MIP, 1);
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
            resultVariables[i] = (essentiallyEqual(values[i], 1, 0.001) ? 1 : 0);
        }

        bool isOptimal = cplex.getStatus() == IloAlgorithm::Optimal;

        return { true, isInfeasible, isOptimal, (int)cplex.getObjValue(), resultVariables, (double)cplex.getMIPRelativeGap() };
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