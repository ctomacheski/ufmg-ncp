#include <vector>
#include <algorithm>
#include <stack>
#include <set>
#include <map>

std::vector<int> getFinishingOrder(std::vector<std::vector<std::pair<int, int> > > adjacencyList) {
    std::set<int> discovered;
    std::set<int> finished;

    std::vector<int> finishingOrder;

    for (int v = 0; v < adjacencyList.size(); v++) {
        std::stack<int> unvisited;
        if (finished.find(v) == finished.end()) {
            unvisited.push(v);
        }

        while(!unvisited.empty()) {
            int visitingVertex = unvisited.top();

            if (discovered.find(visitingVertex) == discovered.end()) {
                discovered.emplace(visitingVertex);

                for(int i = adjacencyList[visitingVertex].size() - 1; i >=0 ; i--) {
                    if (discovered.find(adjacencyList[visitingVertex][i].first) == discovered.end()) {
                        unvisited.push(adjacencyList[visitingVertex][i].first);
                    }
                }
            }
            else {
                if (finished.find(visitingVertex) == finished.end()) {
                    finished.emplace(visitingVertex);
                    finishingOrder.push_back(visitingVertex);
                }
                unvisited.pop();
            }
        }
    }

    std::reverse(finishingOrder.begin(), finishingOrder.end());
    return finishingOrder;
}

std::vector<std::vector<int> > getTransposeAdjacencyList(std::vector<std::vector<std::pair<int, int> > > adjacencyList) {
    std::vector<std::vector<int> > transposeAjacencyList(adjacencyList.size());
    for (int i = 0; i < adjacencyList.size(); i++) {
        for (int j = 0; j < adjacencyList[i].size(); j++) {
            transposeAjacencyList[adjacencyList[i][j].first].push_back(i);
        }
    }

    return transposeAjacencyList;
}

std::map<int, std::set<int> > findStronglyConnectedComponents(std::vector<std::vector<int> > adjacencyList, std::vector<int> finishingOrder) {
    std::set<int> assigned;
    std::map<int, std::set<int> > components;

    for (int i = 0; i < finishingOrder.size(); i++) {
        int finishingVertex = finishingOrder[i];
        std::stack<std::pair<int, int> > notAssigned;
        notAssigned.push(std::make_pair(finishingVertex, finishingVertex));
        while (!notAssigned.empty()) {
            std::pair<int, int> vertex = notAssigned.top();
            notAssigned.pop();

            if (assigned.find(vertex.first) == assigned.end()) {
                if (components.find(vertex.second) != components.end()) {
                    components[vertex.second].insert(vertex.first);
                }
                else {
                    components.emplace(vertex.second, std::set<int>());
                    components[vertex.second].insert(vertex.first);
                }

                assigned.emplace(vertex.first);

                for (int j = 0; j < adjacencyList[vertex.first].size(); j++) {
                    notAssigned.push(std::make_pair(adjacencyList[vertex.first][j], vertex.second));
                }
            }
        }
    }

    return components;
}

std::map<int, std::set<int> > kosarajuAlgorithm(std::vector<std::vector<std::pair<int, int> > > adjacencyList) {
    std::vector<int> finishingOrder = getFinishingOrder(adjacencyList);
    std::vector<std::vector<int> > transposeAdjacencyList = getTransposeAdjacencyList(adjacencyList);

    return findStronglyConnectedComponents(transposeAdjacencyList, finishingOrder);
}
