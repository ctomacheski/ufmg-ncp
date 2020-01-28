#include <ilcplex/ilocplex.h>
#include <vector>

#include "cplex.h"

Result nilcatenationCplex(
    bool localBranch,
    bool isLeft,
    std::vector<int> initialSolution,
    std::vector<std::vector<int> > incidenceMatrix
) {
    IloEnv env;

    try {
        IloModel model(env);

        IloBoolVarArray vars(env);
        IloExpr objective(env);
        for (int j = 0; j < incidenceMatrix.size(); j++) {
            vars.add(IloBoolVar(env));
            objective += vars[j]; 
        }
        model.add(IloMaximize(env, objective));
        objective.end();

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
                model.add(inSolution + notInSolution <= 10);
            }
            else {
                model.add(inSolution + notInSolution > 10);
            }

            inSolution.end();
            notInSolution.end();
        }

        for (int j = 0; j < incidenceMatrix[0].size(); j++) {
            IloExpr constraint(env);
            for (int k = 0; k < incidenceMatrix.size(); k++) {
                constraint += incidenceMatrix[k][j] * vars[k];
            }
            model.add(constraint == 0);
            constraint.end();
        }

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());
        cplex.setParam(IloCplex::TiLim, 45);

        if (!cplex.solve()) {
            std::cout << "Failed to optimize LP." << std::endl;
            throw(-1);
        }

        IloNumArray values(env);
        cplex.getValues(values, vars);

        std::vector<int> resultVariables(incidenceMatrix.size());
        for (long i = 0; i < incidenceMatrix.size(); i++) {
            resultVariables[i] = (values[i] > 0 ? 1 : 0);
        }

        bool isOptimal = cplex.getStatus() == IloAlgorithm::Optimal;

        return { isOptimal, (int)cplex.getObjValue(), resultVariables };
    }
    catch (IloException& e) {
        std::cerr << "Concert exception caught: " << e << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown exception caught" << std::endl;
    }

    env.end();
}

Result nilcatenationCplex(std::vector<std::vector<int> > incidenceMatrix) {
    return nilcatenationCplex(false, false, std::vector<int>(), incidenceMatrix);
}