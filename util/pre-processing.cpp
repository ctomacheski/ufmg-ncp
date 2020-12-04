#include <algorithm>
#include <set>

#include "types.h"
#include "subset.h"
#include "pre-processing.h"

bool findSubsetSumWithEdge(Graph graph, int edgeId, int nodeId, int target)
{
  std::vector<Edge> ingoingEdges(graph.ingoingEdges[nodeId]);
  std::vector<Edge> outgoingEdges(graph.outgoingEdges[nodeId]);

  ingoingEdges.erase(std::remove_if(ingoingEdges.begin(), ingoingEdges.end(), [&edgeId](Edge e) { return e.id == edgeId; }), ingoingEdges.end());
  outgoingEdges.erase(std::remove_if(outgoingEdges.begin(), outgoingEdges.end(), [&edgeId](Edge e) { return e.id == edgeId; }), outgoingEdges.end());

  std::vector<int> subset;
  for (int i = 0; i < ingoingEdges.size(); i++)
  {
    subset.push_back(ingoingEdges[i].weight);
  }

  for (int i = 0; i < outgoingEdges.size(); i++)
  {
    subset.push_back(-outgoingEdges[i].weight);
  }

  return subsetSum(subset, target);
}

void excludeEdges(Graph &graph, std::set<int> edges)
{
  std::vector<std::vector<Edge>> ingoingEdges(graph.ingoingEdges);
  std::vector<std::vector<Edge>> outgoingEdges(graph.outgoingEdges);

  for (int i = 0; i < ingoingEdges.size(); i++)
  {
    ingoingEdges[i].erase(std::remove_if(ingoingEdges[i].begin(), ingoingEdges[i].end(), [edges](Edge e) { return edges.find(e.id) != edges.end(); }), ingoingEdges[i].end());
    outgoingEdges[i].erase(std::remove_if(outgoingEdges[i].begin(), outgoingEdges[i].end(), [edges](Edge e) { return edges.find(e.id) != edges.end(); }), outgoingEdges[i].end());
  }

  std::vector<Edge> incidenceMatrix(graph.incidenceMatrix);
  incidenceMatrix.erase(std::remove_if(incidenceMatrix.begin(), incidenceMatrix.end(), [edges](Edge e) { return edges.find(e.id) != edges.end(); }), incidenceMatrix.end());

  graph = {incidenceMatrix, ingoingEdges, outgoingEdges};
}

void preProcessNodes(Graph &graph)
{
  bool hasNodesToExclude = true;
  while (hasNodesToExclude)
  {
    hasNodesToExclude = false;
    for (int i = 0; i < graph.ingoingEdges.size(); i++)
    {
      std::vector<int> subsetSumInput;

      std::set<int> edgesToExclude;
      for (auto it = graph.ingoingEdges[i].begin(); it != graph.ingoingEdges[i].end(); it++)
      {
        edgesToExclude.insert(it->id);
        subsetSumInput.push_back(it->weight);
      }
      for (auto it = graph.outgoingEdges[i].begin(); it != graph.outgoingEdges[i].end(); it++)
      {
        edgesToExclude.insert(it->id);
        subsetSumInput.push_back(-it->weight);
      }

      if (subsetSumInput.size() > 0)
      {
        bool hasSubsetSum = subsetSum(subsetSumInput, 0);

        if (!hasSubsetSum)
        {
          hasNodesToExclude = true;
          excludeEdges(graph, edgesToExclude);
        }
      }
    }
  }
}

bool preProcessEdges(Graph &graph)
{
  std::vector<Edge> incidenceMatrix = graph.incidenceMatrix;

  for (int e = 0; e < incidenceMatrix.size(); e++)
  {
    Edge edge = incidenceMatrix[e];

    bool haveSolutionNodeFrom = findSubsetSumWithEdge(graph, edge.id, edge.from, edge.weight);
    bool haveSolutionNodeTo = findSubsetSumWithEdge(graph, edge.id, edge.to, -edge.weight);

    if (!(haveSolutionNodeFrom && haveSolutionNodeTo))
    {
      excludeEdges(graph, {edge.id});
      return true;
    }
  }

  return false;
}

void preProcess(Graph &graph)
{
  bool shouldContinue = true;
  while (shouldContinue)
  {
    preProcessNodes(graph);
    shouldContinue = preProcessEdges(graph);
  }
}
