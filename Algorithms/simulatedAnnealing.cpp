//
// Created by Piotr on 11.12.2024.
//

#include "simulatedAnnealing.h"

#include <climits>


void simulatedAnnealing::SAlgorithm(std::vector<std::vector<int>>& graph, int V, float T_max, float T_min, float E_przerwania, float alfa) {//alg zwraca optymalne rozwiazanie
    float T = T_max;//inicjacja poczatkowej temperatury
    repetetiveNearestNeighbour(graph, V);//ustawia mi pola klasy lowest cost i wpisuje najlepsza sciezke
    std::cout<<"Najlepszy koszt: "<<lowestCost<<std::endl;
    for(auto element : bestPath) {
        std::cout<<element<<" ";
    }
    std::cout<<std::endl;
    ////////////////////
    std::vector<int> initalPath = bestPath;

    std::vector<int> bestCosts;
    std::vector<std::vector<int>> bestPaths;

    while (T > T_min) {
        initalPath = bestPath;//przypisanie przed kolejnymi epokami
        for(int i = 0 ; i < V ; i++) {//odpowiada za generowanie wszystkich kombinacji
            for(int j = i + 1; j < V ; j++) {
                std::vector<int> temp = initalPath;
                if(i == 0) {//jesli zamienieamy pierwszy to musimy tez ostatni (do ostatniego nie dochodzi petla)
                    temp[temp.size() - 1] = temp[j];
                }
                std::swap(temp[i], temp[j]);
                int cost = 0;
                for(int k = 0 ; k < V - 1 ; k++) {//zliczanie dlugosci nowej sciezki
                    cost += graph[temp[k]][temp[k+1]];
                }
                cost += graph[temp[V-1]][temp[0]];//koszt powrotu do wierz startowego
                if (cost < lowestCost) {
                    lowestCost = cost;
                    bestPath = temp;//zmiana dotychczasowego naj rozwiazania
                } else {
                    float cost_new = static_cast<float>(cost);
                    float cost_old = static_cast<float>(lowestCost);
                    float p = 1/(1 + std::exp((cost_old - cost_new)/T));
                    std::srand(static_cast<unsigned>(time(0))); // Seed dla generatora liczb pseudolosowych
                    float r = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
                        if(r < p) {
                            bestCosts.push_back(lowestCost);
                            bestPaths.push_back(bestPath);

                            lowestCost = cost;
                            bestPath = temp;//zmiana dotychczasowego naj rozwiazania
                        }
                }
            }
        }

        T *= 0.75;
    }
    for(int i = 0 ; i < bestCosts.size() ; i++) {
        //std::cout<<i<<std::endl;
        if(bestCosts[i] < lowestCost) {
            lowestCost = bestCosts[i];
            bestPath = bestPaths[i];
        }
    }



    std::cout<<"Najlepszy koszt: "<<lowestCost<<std::endl;
    for(auto element : bestPath) {
        std::cout<<element<<" ";
    }
    std::cout<<std::endl;


}




int simulatedAnnealing::nearestNeighbour(std::vector<std::vector<int>>& graph, int V, int start, std::vector<bool>& odwiedzone,
                          int current_cost, std::vector<int>& current_path,
                          std::vector<int>& best_path, int& best_cost) {
    if (odwiedzone[start] == false) {//ustawienie nextVertex przeslanego w rekrencyjnym wywolaniu na odwiedzony
        odwiedzone[start] = true;
        current_path.push_back(start);  // Add the current node to the path
    }

    bool czyOdwiedzoneWszystkie = ifAllVisited(odwiedzone);

    if (czyOdwiedzoneWszystkie) {//jesli cala droga juz jest znaleziona wystarczy juz tylko polaczyc z wierzcholkiem startowym
        // Check the weight of returning to the start node
        int last_weight = graph[current_path.back()][current_path[0]];
        if (last_weight == -1) {
            return -1;  // zla sciezka, bo nie moze wrocic do poczatkowego wierzxholka
        }

        int total_cost = current_cost + last_weight;
        //current_path.push_back(current_path[0]);

        // Update the best path if a better solution is found
        if (total_cost < best_cost) {
            best_cost = total_cost;
            best_path = current_path;  // Save the current path as the best
        }

        return best_cost;
    }

    // List of potential candidates with the minimum weight
    std::vector<int> candidates;
    int minWeight = INT_MAX;

    // Select the candidates with the minimum weight
    for (int v = 0; v < graph[start].size(); ++v) { // Iterujemy po wszystkich wierzchołkach
        if (!odwiedzone[v] && graph[start][v] != -1) { // Sprawdzamy, czy wierzchołek nieodwiedzony i krawędź istnieje
            if (graph[start][v] < minWeight) { // Jeśli waga jest mniejsza niż dotychczasowe minimum
                minWeight = graph[start][v];
                candidates.clear();
                candidates.push_back(v); // Dodajemy wierzchołek jako jedynego kandydata
            } else if (graph[start][v] == minWeight) { // Jeśli waga jest taka sama jak minimum
                candidates.push_back(v); // Dodajemy wierzchołek do listy kandydatów
            }
        }
    }

    // eksploracja wszystkich rozglezien
    for (int nextVertex : candidates) {
        odwiedzone[nextVertex] = true;
        current_path.push_back(nextVertex);  // Add the vertex to the current path

        // current_cost + minWeight to nowy current cost w kolejnym wowlaniu
        nearestNeighbour(graph, V, nextVertex, odwiedzone, current_cost + minWeight, current_path, best_path, best_cost);

        // Backtracking i przygotowanie przed zwiedzeniem koljengo rozgalezienia
        odwiedzone[nextVertex] = false;
        current_path.pop_back();  // Remove the last vertex from the current path
    }

    return best_cost;
}


int simulatedAnnealing::repetetiveNearestNeighbour(std::vector<std::vector<int>>& graph, int V) {
    std::vector<bool> odwiedzone(V, false);       // Na początku wszystkie wierzchołki nieodwiedzone
    int current_cost = 0;                         // Początkowy koszt trasy to 0
    std::vector<int> current_path;                // Początkowa trasa jest pusta
    std::vector<int> best_path;                   // Najlepsza trasa, którą chcemy znaleźć
    int best_cost = INT_MAX;                      // Początkowy najlepszy koszt ustawiony na nieskończonoś
    //powyzej dane potrzebne do odpalenia rekurecyjnej metody nearestneighbour

    int best_weight = -1;//dajemy wartosc ujemna zeby przy pierwszej iteracji zostala ta zmienna nadpisana
    std::vector<int> best_way;
    for(int i = 0 ; i < V ; i++) {

        int result = nearestNeighbour(graph, V, i, odwiedzone, current_cost, current_path, best_path, best_cost);


        //przygotowanie przed kolejna rundą
        odwiedzone.clear();
        odwiedzone.assign(V, false);
        current_cost = 0;
        current_path.clear();

        //i to wierzcholek startowy dla metody nearestneighbour
        if(result != -1 && (best_weight == -1 || best_weight > result)) {
            best_weight = best_cost;
            best_way = best_path;
        }

    }
    lowestCost = best_weight;//dopiero po calym repetetive nearestneighbour nadpisujemy wartosc pol klasy
    bestPath = best_way;
    bestPath.push_back(bestPath[0]);
    return lowestCost;
}

bool simulatedAnnealing::ifAllVisited(std::vector<bool> odwiedzone) {//metoda pomocniczna do NN
    for(int i = 0 ; i < odwiedzone.size() ; i++) {
        if (odwiedzone[i] == false) {
            return false;
        }
    }
    return true;

}