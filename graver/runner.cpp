#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>

std::vector<std::vector<int> > parseIncidenceMatrix(int vertexCount, int edgeCount) {
    std::vector<std::vector<int> > incidenceMatrix(vertexCount, std::vector<int>(edgeCount));

    for (int i = 0; i < edgeCount; i++) {
        int from, to, weight;
        std::cin >> from >> to >> weight;

        incidenceMatrix[from][i] = -weight;
        incidenceMatrix[to][i] = weight;
    }

    return incidenceMatrix;
}

int main (int argc, char **argv) {
    std::string fileName;
    if (argc == 2) {
        fileName = std::string(argv[1]);
    }

    int nodeCount;
    int edgeCount;
    std::cin >> nodeCount >> edgeCount;

    std::vector<std::vector<int> > incidenceMatrix = parseIncidenceMatrix(nodeCount, edgeCount);

    std::ofstream matrixFile;
    matrixFile.open(fileName + ".mat");

    matrixFile << nodeCount << " " << edgeCount << "\n";

    for (int i = 0; i < nodeCount; i++) {
      for (int j = 0; j < edgeCount; j++) {
        matrixFile << incidenceMatrix[i][j] << " ";
      }
      matrixFile << "\n";
    }
  
    matrixFile.close();

    std::string command = "graver " + fileName;
    system(command.c_str());

    return 0; 
}