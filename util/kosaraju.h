#ifndef KOSARAJU_H
#define KOSARAJU_H

#include <map>
#include <set>
#include <vector>

std::map<int, std::set<int> > kosarajuAlgorithm(std::vector<std::vector<std::pair<int, int> > > adjacencyList);

#endif