#include <vector>
#include <ilcp/cp.h>

struct Edge {
    int from;
    int to;
    int weight;
};

std::vector<Edge> parseInputGraph(int m) {
    std::vector<Edge> edges(m);

    for (int e = 0; e < m; e++) {
        int i, j, w;
        std::cin >> i >> j >> w;

        Edge ed;
        ed.from = i;
        ed.to = j;
        ed.weight = w;
        edges[e] = ed;
    }

    return edges;
}

int main() {
    int n, m;
    std::cin >> n >> m;

    std::vector<Edge> edges = parseInputGraph(m);

    IloEnv env;
    try {
        IloModel model(env);
        IloIntVarArray variables(env);
        IloExprArray restrictionIn(env);
        IloExprArray restrictionOut(env);

        for (long i = 0; i < n; i++) {
            restrictionIn.add(IloExpr(env));
            restrictionOut.add(IloExpr(env));
        }

        for (long i = 0; i < edges.size(); i++) {
            variables.add(IloIntVar(env, 0, 1));

            Edge e = edges[i];
            restrictionOut[e.from] += variables[i] * e.weight;
            restrictionIn[e.to] += variables[i] * e.weight;
        }

        for (long i = 0; i < n; i++) {
            model.add(restrictionIn[i] == restrictionOut[i]);
        }

        model.add(IloMaximize(env, IloSum(variables)));

        IloCP cp(model);
        cp.setOut(env.getNullStream());
        cp.setParameter(IloCP::TimeLimit, 30);
        cp.propagate();

        if (cp.solve()) {
            std::cout << cp.getObjValue() << " ";
            bool isOptimal = cp.getStatus() == IloAlgorithm::Optimal;
            std::cout << (isOptimal ? "1" : "0") << std::endl;

            for (long i = 0; i < m; i++) {
              std::cout << cp.getValue(variables[i]);
              if (i != m - 1) std::cout << " ";
            }
            std::cout << std::endl;
        }
    }
    catch (IloException &ex) {
        std::cout << "Error: " << ex << std::endl;
    }
    env.end();

    return 0;
}