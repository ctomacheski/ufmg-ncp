#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <limits>

struct Edge {
  int id;
  int from;
  int to;
  int weight;
};

struct Node {
  std::vector<Edge> ingoing;
  std::vector<Edge> outgoing;
};

struct SolutionEraseInsert {
  int enhancement;
  int id;
};

struct SolutionSwap {
  int enhancement;
  int idInsert;
  int idRemove;
};

void parseData(std::map<int, Node> &graph, std::vector<Edge> &edges, int m) {
  for (int i = 0; i < m; i++) {
    int from, to, weight;
    std::cin >> from >> to >> weight;
    Edge e = { i, from, to, weight };

    graph[from].outgoing.push_back(e);
    graph[to].ingoing.push_back(e);

    edges.push_back(e);
  }
}

bool isSolution(std::vector<int> ingoingWeights, std::vector<int> outgoingWeights, int n) {
  bool zeroSum = true;
  for (int i = 0; i < n; i++) {
      zeroSum &= ingoingWeights[i] - outgoingWeights[i] == 0;
  }

  return zeroSum;
}

void calculateBalance(std::map<int, Node> graph, std::set<int> edgesInSolution, std::vector<int> &ingoingWeights,
  std::vector<int> &outgoingWeights, int n) {
  for (int i = 0; i < n; i++) {
      ingoingWeights[i] = 0;
      outgoingWeights[i] = 0;
  }

  for (int i = 0; i < n; i++) {
    Node n = graph[i];
    for (int j = 0; j < n.ingoing.size(); j++) {
      Edge e = n.ingoing[j];
      if (edgesInSolution.find(e.id) != edgesInSolution.end()) {
        ingoingWeights[i] += e.weight;
      }
    }

    for (int j = 0; j < n.outgoing.size(); j++) {
      Edge e = n.outgoing[j];
      if (edgesInSolution.find(e.id) != edgesInSolution.end()) {
        outgoingWeights[i] += e.weight;
      }
    }
  }
}

bool insertEdgeNeighborhood(std::set<int> &edgesInSolution, std::set<int> &edgesNotInSolution,
  std::vector<int> ingoingWeights, std::vector<int> outgoingWeights, std::vector<Edge> edges) {

  SolutionEraseInsert solution = { std::numeric_limits<int>::min(), -1 };
  bool isImproving = false;

  for (auto it = edgesNotInSolution.begin(); it != edgesNotInSolution.end(); it++) {
    Edge e = edges[*it];
    int ingoingDiference = ingoingWeights[e.from] - outgoingWeights[e.from];
    int outgoingDiference = ingoingWeights[e.to] - outgoingWeights[e.to];

    if (outgoingDiference < 0 && ingoingDiference > 0) {
      int newValue = std::abs(outgoingDiference + e.weight) + std::abs(ingoingDiference - e.weight);
      int oldValue = std::abs(outgoingDiference) + std::abs(ingoingDiference);
      if (solution.enhancement < oldValue - newValue) {
        solution = { oldValue - newValue, e.id };
        isImproving = true;
      }
    }
  }

  if (isImproving) {
    edgesInSolution.insert(solution.id);
    edgesNotInSolution.erase(solution.id);
  }

  return isImproving;
}

bool removeEdgeNeighborhood(std::set<int> &edgesInSolution, std::set<int> &edgesNotInSolution,
  std::vector<int> ingoingWeights, std::vector<int> outgoingWeights, std::vector<Edge> edges) {

  SolutionEraseInsert solution = { std::numeric_limits<int>::min(), -1 };
  bool isImproving = false;

  for (auto it = edgesInSolution.begin(); it != edgesInSolution.end(); it++) {
    Edge e = edges[*it];
    int ingoingDiference = ingoingWeights[e.from] - outgoingWeights[e.from];
    int outgoingDiference = ingoingWeights[e.to] - outgoingWeights[e.to];

    if (outgoingDiference > 0 && ingoingDiference < 0) {
      int newValue = std::abs(outgoingDiference - e.weight) + std::abs(ingoingDiference + e.weight);
      int oldValue = std::abs(outgoingDiference) + std::abs(ingoingDiference);
      if (solution.enhancement < oldValue - newValue) {
        solution = { oldValue - newValue, e.id };
        isImproving = true;
      }
    }
  }

  if (isImproving) {
    edgesNotInSolution.insert(solution.id);
    edgesInSolution.erase(solution.id);
  }

  return isImproving;
}

int main() {
  int n, m;
  std::cin >> n >> m;

  std::map<int, Node> graph;
  std::vector<Edge> edges;
  parseData(graph, edges, m);

  std::vector<int> ingoingWeights(n);
  std::vector<int> outgoingWeights(n);

  std::set<int> edgesInSolution;
  for (int i = 0; i < m; i++) {
    edgesInSolution.insert(i);
  }

  std::set<int> edgesNotInSolution;

  calculateBalance(graph, edgesInSolution, ingoingWeights, outgoingWeights, n);

  bool isImproving = true;
  bool foundNilcatenation = false;
  while (isImproving && !foundNilcatenation) {
    isImproving = removeEdgeNeighborhood(edgesInSolution, edgesNotInSolution,
      ingoingWeights, outgoingWeights, edges);

    calculateBalance(graph, edgesInSolution, ingoingWeights, outgoingWeights, n);

    foundNilcatenation |= isSolution(ingoingWeights, outgoingWeights, n);
  }

  if (!foundNilcatenation) {
    isImproving = true;
    foundNilcatenation = false;
    while (isImproving && !foundNilcatenation) {
      isImproving = insertEdgeNeighborhood(edgesInSolution, edgesNotInSolution,
          ingoingWeights, outgoingWeights, edges);

      calculateBalance(graph, edgesInSolution, ingoingWeights, outgoingWeights, n);

      foundNilcatenation |= isSolution(ingoingWeights, outgoingWeights, n);
    }
  }

  foundNilcatenation = isSolution(ingoingWeights, outgoingWeights, n);
  std::cout << (foundNilcatenation ? "1" : "0" ) << " " << edgesInSolution.size() << std::endl;

  for (int i = 0; i < m; i++) {
    if (edgesInSolution.find(i) != edgesInSolution.end()) {
      std::cout << "1";
    }
    else {
      std::cout << "0";
    }
    std::cout << " ";
  }

  std::cout << std::endl;

  return 0;
}