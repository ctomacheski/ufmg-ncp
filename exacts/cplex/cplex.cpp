#include <ilcplex/ilocplex.h>
#include <vector>
#include <set>

#include "cplex.h"

Graph parseIncidenceMatrix(int vertexCount, int edgeCount)
{
    std::vector<Edge> incidenceMatrix(edgeCount);

    std::vector<std::vector<Edge>> ingoingEdges(vertexCount, std::vector<Edge>());
    std::vector<std::vector<Edge>> outgoingEdges(vertexCount, std::vector<Edge>());

    for (int i = 0; i < edgeCount; i++)
    {
        int from, to;
        unsigned long weight;
        std::cin >> from >> to >> weight;

        incidenceMatrix[i].id = i;
        incidenceMatrix[i].from = from;
        incidenceMatrix[i].to = to;
        incidenceMatrix[i].weight = weight;

        ingoingEdges[to].push_back(incidenceMatrix[i]);
        outgoingEdges[from].push_back(incidenceMatrix[i]);
    }

    return {incidenceMatrix, ingoingEdges, outgoingEdges};
}

bool verifySolution(std::vector<Edge> incidenceMatrix, Result result, int nodeCount)
{
    std::vector<unsigned long> graphBalance(nodeCount, 0);

    for (int i = 0; i < result.variablesResult.size(); i++)
    {
        graphBalance[incidenceMatrix[i].from] -= incidenceMatrix[i].weight * result.variablesResult[i];
        graphBalance[incidenceMatrix[i].to] += incidenceMatrix[i].weight * result.variablesResult[i];
    }

    bool isCorrect = true;
    for (int i = 0; i < graphBalance.size(); i++)
    {
        isCorrect &= graphBalance[i] == 0;
        if (graphBalance[i] != 0)
            std::cout << i << "," << graphBalance[i] << std::endl;
        ;
    }

    return isCorrect;
}

bool essentiallyEqual(float a, float b, float epsilon)
{
    return fabs(a - b) <= ((fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

ILOMIPINFOCALLBACK3(MIPInfoCallback, bool, isMinimization, IloCplex, cplex, IloNum, startTime)
{
    if (!hasIncumbent())
        return;

    static double prevBest = -1e+100;
    const IloNum currentBest = getIncumbentObjValue();

    static IloNum internalStartTime = 1e+100;
    if (internalStartTime > startTime)
    {
        internalStartTime = startTime;
    }

    if (currentBest > prevBest)
    {
        prevBest = currentBest;
        std::cout << (cplex.getCplexTime() - internalStartTime) << " " << currentBest << std::endl;
    }
};

Result nilcatenationCplex(
    Graph graph,
    int nodeCount,
    int timeLimit,
    bool useLB)
{
    std::vector<Edge> incidenceMatrix = graph.incidenceMatrix;

    IloEnv env;

    try
    {
        IloModel model(env);

        IloIntVarArray vars(env);
        IloExpr objective(env);
        for (int i = 0; i < incidenceMatrix.size(); i++)
        {
            vars.add(IloIntVar(env, 0, 1));
            objective += vars[i];
        }

        model.add(objective > 0);
        model.add(IloMaximize(env, objective));

        IloExprArray conditionNilc(env);
        for (int i = 0; i < nodeCount; i++)
        {
            conditionNilc.add(IloExpr(env));
        }

        std::vector<bool> hasConditionFrom(nodeCount, false);
        std::vector<bool> hasConditionTo(nodeCount, false);

        for (int i = 0; i < incidenceMatrix.size(); i++)
        {
            Edge e = incidenceMatrix[i];
            conditionNilc[e.from] -= e.weight * vars[i];
            conditionNilc[e.to] += e.weight * vars[i];

            hasConditionFrom[e.from] = true;
            hasConditionTo[e.to] = true;
        }

        for (int i = 0; i < nodeCount; i++)
        {
            if (hasConditionTo[i] && hasConditionFrom[i])
            {
                model.add(conditionNilc[i] == 0);
            }
        }

        IloCplex cplex(model);
        const IloNum startTime = cplex.getCplexTime();

        cplex.setOut(env.getNullStream());
        cplex.setParam(IloCplex::TiLim, timeLimit);
        cplex.setParam(IloCplex::Param::RandomSeed, 1);

        if (useLB)
        {
            cplex.setParam(IloCplex::Param::MIP::Strategy::LBHeur, 1);
        }

        cplex.use(MIPInfoCallback(env, false, cplex, startTime));

        if (!cplex.solve())
        {
            return {false};
        }

        bool isInfeasible = cplex.getStatus() == IloAlgorithm::Infeasible;
        if (isInfeasible)
        {
            return {true, isInfeasible};
        }

        IloNumArray values(env);
        cplex.getValues(values, vars);

        std::vector<int> resultVariables(incidenceMatrix.size());
        for (long i = 0; i < incidenceMatrix.size(); i++)
        {
            resultVariables[i] = (essentiallyEqual(values[i], 1, 0.001) ? 1 : 0);
        }

        bool isOptimal = cplex.getStatus() == IloAlgorithm::Optimal;

        return {true, isInfeasible, isOptimal, (int)cplex.getObjValue(), resultVariables, (double)cplex.getMIPRelativeGap()};
    }
    catch (IloException &e)
    {
        std::cerr << "Concert exception caught: " << e << std::endl;
    }
    catch (...)
    {
        std::cerr << "Unknown exception caught" << std::endl;
    }

    return {false};

    env.end();
}

Result nilcatenationCplex(
    std::vector<Edge> incidenceMatrix,
    int nodeCount,
    int timeLimit,
    int lowerBound,
    bool returnFirstSolution,
    int solutionCount,
    std::vector<BranchingConstraint> branchingContraints)
{
    IloEnv env;

    try
    {
        IloModel model(env);

        IloBoolVarArray vars(env);
        IloExpr objective(env);
        for (int i = 0; i < incidenceMatrix.size(); i++)
        {
            vars.add(IloBoolVar(env));
            objective += vars[i];
        }

        model.add(objective > 0);
        model.add(IloMaximize(env, objective));

        for (BranchingConstraint localBranching : branchingContraints)
        {
            IloExpr inSolution(env);
            IloExpr notInSolution(env);

            for (int i = 0; i < localBranching.referenceSolution.size(); i++)
            {
                if (localBranching.referenceSolution[i] == 1)
                {
                    inSolution += (1 - vars[i]);
                }
                else
                {
                    notInSolution += vars[i];
                }
            }

            if (localBranching.isLeft)
            {
                model.add(inSolution + notInSolution <= localBranching.rhs);
            }
            else
            {
                model.add(inSolution + notInSolution >= localBranching.rhs);
            }
        }

        IloExprArray constraints(env);
        for (int i = 0; i < nodeCount; i++)
        {
            constraints.add(IloExpr(env));
        }

        for (int i = 0; i < incidenceMatrix.size(); i++)
        {
            Edge e = incidenceMatrix[i];
            constraints[e.from] -= e.weight * vars[i];
            constraints[e.to] += e.weight * vars[i];
        }

        for (int i = 0; i < nodeCount; i++)
        {
            model.add(constraints[i] == 0);
        }

        IloCplex cplex(model);
        const IloNum startTime = cplex.getCplexTime();

        //cplex.setOut(env.getNullStream());
        cplex.setParam(IloCplex::TiLim, timeLimit);

        if (lowerBound > 0)
        {
            cplex.setParam(IloCplex::Param::MIP::Tolerances::LowerCutoff, std::max(lowerBound, 0) + 1);
        }

        cplex.setParam(IloCplex::Param::RandomSeed, 1);

        if (returnFirstSolution)
        {
            cplex.setParam(IloCplex::Param::MIP::Limits::Solutions, solutionCount);
            // cplex.setParam(IloCplex::Param::Emphasis::MIP, 1);
        }

        cplex.use(MIPInfoCallback(env, false, cplex, startTime));

        if (!cplex.solve())
        {
            return {false};
        }

        bool isInfeasible = cplex.getStatus() == IloAlgorithm::Infeasible;
        if (isInfeasible)
        {
            return {true, isInfeasible};
        }

        IloNumArray values(env);
        cplex.getValues(values, vars);

        std::vector<int> resultVariables(incidenceMatrix.size());
        for (long i = 0; i < incidenceMatrix.size(); i++)
        {
            resultVariables[i] = (essentiallyEqual(values[i], 1, 0.001) ? 1 : 0);
        }

        bool isOptimal = cplex.getStatus() == IloAlgorithm::Optimal;

        return {true, isInfeasible, isOptimal, (int)cplex.getObjValue(), resultVariables, (double)cplex.getMIPRelativeGap()};
    }
    catch (IloException &e)
    {
        std::cerr << "Concert exception caught: " << e << std::endl;
    }
    catch (...)
    {
        std::cerr << "Unknown exception caught" << std::endl;
    }

    return {false};

    env.end();
}