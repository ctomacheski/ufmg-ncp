#include <ilcplex/ilocplex.h>
#include <vector>
#include <chrono>

std::vector<std::vector<int> > parseIncidenceMatrix(int vertexCount, int edgeCount) {
    std::vector<std::vector<int> > incidenceMatrix(edgeCount, std::vector<int>(vertexCount));

    for (int i = 0; i < edgeCount; i++) {
        int from, to, weight;
        std::cin >> from >> to >> weight;

        incidenceMatrix[i][from] = -weight;
        incidenceMatrix[i][to] = weight;
    }

    return incidenceMatrix;
}

int main (int argc, char **argv)
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    
    int vertexCount;
    int edgeCount;
    std::cin >> vertexCount >> edgeCount;

    std::vector<std::vector<int> > incidenceMatrix = parseIncidenceMatrix(vertexCount, edgeCount);

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

        for (int j = 0; j < incidenceMatrix[0].size(); j++) {
            IloExpr constraint(env);
            for (int k = 0; k < incidenceMatrix.size(); k++) {
                constraint += incidenceMatrix[k][j] * vars[k];
            }
            model.add(constraint == 0);
            constraint.end();
        }

        IloCplex cplex(model);
        cplex.setParam(IloCplex::TiLim, 300);

        if (!cplex.solve()) {
            std::cout << "Failed to optimize LP." << std::endl;
            throw(-1);
        }

        IloNumArray vals(env);
        std::cout << "Solution status = " << cplex.getStatus() << std::endl;
        std::cout << "Solution value = " << cplex.getObjValue() << std::endl;
        cplex.getValues(vals, vars);
        std::cout << "Values = " << vals << std::endl;
    }
    catch (IloException& e) {
        std::cerr << "Concert exception caught: " << e << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown exception caught" << std::endl;
    }

    env.end();
    
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time: " <<std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << std::endl;
}
