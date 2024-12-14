//
// Created by Piotr on 11.12.2024.
//

#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H
#include <vector>
#include <cmath>
#include <iostream>


class simulatedAnnealing {

private:
    int lowestCost;
    std::vector<int> bestPath;
    int repetetiveNearestNeighbour(std::vector<std::vector<int>>& graph, int V);
    int nearestNeighbour(std::vector<std::vector<int>>& graph, int V, int start, std::vector<bool>& odwiedzone,
                          int current_cost, std::vector<int>& current_path,
                          std::vector<int>& best_path, int& best_cost);
    bool ifAllVisited(std::vector<bool> odwiedzone);

public:
    void SAlgorithm(std::vector<std::vector<int>>& graph, int V, float T_max, float T_min, float E_przerwania, float alfa);

};



#endif //SIMULATEDANNEALING_H
