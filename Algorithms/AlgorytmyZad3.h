//
// Created by Piotr on 11.12.2024.
//

#ifndef ALGORYTMYZAD3
#define ALGORYTMYZAD3
#include <vector>
#include <cmath>
#include <iostream>
#include <list>
#include <random>
#include <algorithm>
#include "../mainProgram/Timer.h"

class AlgorytmyZad3 {

private:
    Timer& timer_;
    int lowestCost;
    std::vector<int> bestPath;
    bool generateInitSolutionWithNn;
    bool ifGenerateNeighbourhoodWithSwap;
    bool ifGeometricCooling;
    double procentageOfLowerBound;
    int iterationsToTakeWorse;
    int iterationsWithoutImprove;
    int solutionFromFile;
    int repetetiveNearestNeighbour(std::vector<std::vector<int>>& graph, int V);
    int nearestNeighbour(std::vector<std::vector<int>>& graph, int V, int start, std::vector<bool>& odwiedzone,
                          int current_cost, std::vector<int>& current_path,
                          std::vector<int>& best_path, int& best_cost);
    void randomMethod(std::vector<std::vector<int>>& graph, int V);
    bool ifAllVisited(std::vector<bool> odwiedzone);
    void printCostnPath();
    bool ifInTabuList(std::vector<std::pair<std::vector<int>, int>>&tabuList, std::vector<int> rozwiazanie);
    void decrementCadency(std::vector<std::pair<std::vector<int>, int>>&tabuList);
    int swapMethod(std::vector<std::vector<int>>& graph, int V, std::vector<int>& temp, int i, int j);
    int twoOpt(std::vector<std::vector<int>>& graph, int V, std::vector<int>& temp, int i, int j);
    bool ifOptimumFound();
    bool ifInProcentageOfLowerBound();
public:
    AlgorytmyZad3(Timer&, bool ifStartWithNN, bool ifGenerateWithSwap, bool, int, int, int, double);
    void SAlgorithm(std::vector<std::vector<int>>& graph, int V, double T_max, double T_min, double alfa);
    void TS(std::vector<std::vector<int>>& graph, int V, int sizeOfTabuList, int kadencja);
    int getLowestCost();
    std::vector<int> getBestPath();
    double countRelativeError();//blad względny
    int countAbsoluteError();//blad bezwzgledny
};



#endif //ALGORYTMYZAD3
