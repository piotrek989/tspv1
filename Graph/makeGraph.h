//
// Created by Piotr on 10.12.2024.
//

#ifndef MAKEGRAPH_H
#define MAKEGRAPH_H
#include <vector>
#include <iostream>
#include <random>
#include <ctime>
#include <fstream>
#include <iomanip> // dla precyzji zapisu liczb

class makeGraph {


public:
    makeGraph();
    void generateGraph(std::vector<std::vector<int>>& graph, int density, bool isDirected, int V);
    void getFromFile(std::string nazwaPlikuWe, std::vector<std::vector<int>>&, int&, int&);
    bool isSymetric(std::vector<std::vector<int>>& graph, int V);
    void printGraph(std::vector<std::vector<int>>& graph);
    static void writeToFileTimesAndAvg(std::vector<double> times, std::vector<int> absoluteErrors,
                                 std::vector<double> relativeErrors,
                                 std::vector<int> solutions,
                                 double avgTime, double absErrorAvg, double relativeErrorAvg);
    static void writeInitInfo(const std::string& nameInputFile, const std::string& typeOfMethod, int solutionFromFile);
};



#endif //MAKEGRAPH_H
