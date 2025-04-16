//
// Created by Piotr on 11.12.2024.
//

#include "AlgorytmyZad3.h"

#include <climits>

AlgorytmyZad3::AlgorytmyZad3(Timer& timer, bool ifStartWithNN, bool ifGenerateWithSwap, bool ifgeometriColling, int ery, int solFromFile, int iterwithoutimprove,double procOfLowerBoud)
    :timer_(timer),
    generateInitSolutionWithNn(ifStartWithNN),
    ifGenerateNeighbourhoodWithSwap(ifGenerateWithSwap),
    ifGeometricCooling(ifgeometriColling),
    eras(ery),
    solutionFromFile(solFromFile),
    iterationsWithoutImprove(iterwithoutimprove),
    procentageOfLowerBound(procOfLowerBoud)
    {}


void AlgorytmyZad3::SAlgorithm(std::vector<std::vector<int>>& graph, int V, double T_max, double T_min, double alfa) {
    std::srand(static_cast<unsigned>(time(0))); // Seed dla generatora liczb pseudolosowych
    if(generateInitSolutionWithNn) {
        repetetiveNearestNeighbour(graph, V);//ustawia mi pola klasy lowest cost i wpisuje najlepsza sciezke
    }else{
        randomMethod(graph, V);
    }

    int LB = Prim(graph);
    // std::cout<<"LB: "<<LB<<std::endl;
    double T;
    int k = 0;
    if(ifGeometricCooling) {
        T = T_max;//inicjacja poczatkowej temperatury
    }else{
        T = T_max/log(1.0 + static_cast<double>(k));
    }

    // printCostnPath();
    int x_a = lowestCost;
    std::vector<int> path_a = bestPath;
    //3 + 2(ponizej w kodzie) warunki stopu algorytmu
    while (T > T_min && !ifOptimumFound() && !ifInProcentageOfLowerBound(LB) && timer_.getCounter() < timer_.time_limit && k < iterationsWithoutImprove) {

        if (!ifGeometricCooling)//tylko jesli chlodzenie logarytmiczne
            T = T_max/log(1.0 + static_cast<double>(k));

        int epoka = 0;

        while (epoka < eras) {

            int newCost;
            std::vector<int> newPath;

            if(ifGenerateNeighbourhoodWithSwap) {//w zaleznosci od tej zmiennej generujemy sasiedztwo
                newPath = swap(path_a, V);//metoda swapujaca
                newCost = countingPath(path_a, graph, V);//koszt nowej sciezki
            }else{
                newPath = betterTwoOpt(path_a, V);//metoda swapujaca
                newCost = countingPath(path_a, graph, V);//koszt nowej sciezki
            }
            if(newCost < lowestCost){//poprawianie najlepszego z najlepszych
                lowestCost = newCost;
                bestPath = newPath;
                k = 0;
                // std::cout<<lowestCost<<std::endl;
            }
            if(newCost < x_a) {
                x_a = newCost;
                path_a = newPath;
            } else {
                auto cost_new = static_cast<double>(newCost);
                auto cost_old = static_cast<double>(lowestCost);
                double p = 1.0/(1.0 + std::exp((cost_old - cost_new)/T));
                auto r = static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX);
                if(r < p) {
                    x_a = newCost;
                    path_a = newPath;
                }
            }

            epoka++;
        }
        k++;

        if (ifGeometricCooling)//tylko jesli chlodzenie geo
            T *= alfa;
    }
    //printCostnPath();
}

void AlgorytmyZad3::TS(std::vector<std::vector<int>>& graph, int V, int sizeOfTabuList, int kadencja) {
    if (generateInitSolutionWithNn) {
        repetetiveNearestNeighbour(graph, V); // Ustawia lowestCost i bestPath
    } else {
        randomMethod(graph, V);
    }

    int LB = Prim(graph);

    std::vector<std::pair<std::vector<int>, int>> tabuList;
    int x_a = lowestCost;
    std::vector<int> path_a = bestPath;

    int k = 0;
    while (k < iterationsWithoutImprove && !ifOptimumFound() && !ifInProcentageOfLowerBound(LB) && timer_.getCounter() < timer_.time_limit) {
        std::vector<int> bestNeighbourPath;
        int bestNeighbourCost = INT_MAX;

        for (int i = 0; i < V - 1; i++) { // Generowanie sąsiedztwa
            for (int j = i + 1; j < V; j++) {
                std::vector<int> tmpPath = path_a;
                int tmpCost;

                if (ifGenerateNeighbourhoodWithSwap) {
                    tmpCost = swapMethod(graph, V, tmpPath, i, j);
                } else {
                    tmpCost = twoOpt(graph, V, tmpPath, i, j);
                }

                bool isOnTabuList = ifInTabuList(tabuList, tmpPath);
                int mval = tmpCost; // Funkcja MVal, tutaj prosto zdefiniowana jako koszt

                if ((!isOnTabuList && mval < bestNeighbourCost) || (isOnTabuList && tmpCost < lowestCost)) {
                    bestNeighbourCost = tmpCost;
                    bestNeighbourPath = tmpPath;
                }
            }
        }

        if (!bestNeighbourPath.empty()) {
            path_a = bestNeighbourPath;
            x_a = bestNeighbourCost;

            if (x_a < lowestCost) {
                lowestCost = x_a;
                bestPath = path_a;
                k = 0; // Reset iteracji bez poprawy
            } else {
                k++; // Inkrementacja, jeśli nie poprawiono najlepszego rozwiązania
            }

            // Dodanie do listy tabu
            tabuList.emplace_back(path_a, kadencja);
            if (tabuList.size() > sizeOfTabuList) {
                tabuList.erase(tabuList.begin()); // Usuwanie najstarszego elementu
            }
        }

        // Aktualizacja kadencji na liście tabu
        for (auto it = tabuList.begin(); it != tabuList.end();) {
            it->second--;
            if (it->second <= 0) {
                it = tabuList.erase(it);
            } else {
                ++it;
            }
        }
    }
}

bool AlgorytmyZad3::ifInTabuList(std::vector<std::pair<std::vector<int>, int>> &tabuList, std::vector<int> rozwiazanie) {
    for (const auto& tabuElement : tabuList) {
        if (tabuElement.first == rozwiazanie) {
            return true;
        }
    }
    return false;
}


void AlgorytmyZad3::decrementCadency(std::vector<std::pair<std::vector<int>, int> > &tabuList) {
    int n = tabuList.size();
    for(int i = 0 ; i < n ; i++) {
        tabuList[i].second--;
        if(tabuList[i].second == 0)
            tabuList.erase(tabuList.begin() + i);
    }
}


void AlgorytmyZad3::printCostnPath() {
    if(!bestPath.empty()) {
        std::cout<<"Oto koszt: "<<lowestCost<<std::endl;
        for(auto element : bestPath) {
            std::cout<<element<<" ";
        }
        std::cout<<std::endl;
    } else
        std::cout<<"Lista jest pusta - BRAK WYNIKOW!!!"<<std::endl;
}


int AlgorytmyZad3::nearestNeighbour(std::vector<std::vector<int>>& graph, int V, int start, std::vector<bool>& odwiedzone,
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

        if (total_cost < best_cost) {
            best_cost = total_cost;
            best_path = current_path;
        }

        return best_cost;
    }

    // List of potential candidates with the minimum weight
    std::vector<int> candidates;
    int minWeight = INT_MAX;

    // Select the candidates with the minimum weight
    for (int v = 0; v < graph[start].size(); ++v) {
        if (!odwiedzone[v] && graph[start][v] != -1) {
            if (graph[start][v] < minWeight) {
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


int AlgorytmyZad3::repetetiveNearestNeighbour(std::vector<std::vector<int>>& graph, int V) {
    std::vector<bool> odwiedzone(V, false);// Na początku wszystkie wierzchołki nieodwiedzone
    int current_cost = 0;
    std::vector<int> current_path;
    std::vector<int> best_path;
    int best_cost = INT_MAX;

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

void AlgorytmyZad3::randomMethod(std::vector<std::vector<int>> &graph, int V) {
    std::random_device rd;  // rd jest wykorzystywane do generowania losowej wartości (ziarno)
    std::mt19937 g(rd());   // tworze generator liczb pseudolosowych, inicjalizowany wartością z urządzenia losowego

    lowestCost = INT_MAX;//poczatkowa wartosc lowestCost
    std::vector<int> tab_nieodwiedzonych(V);

    for (int i = 0; i < V; i++) {  // indeksów tyle, ile wierzchołków
        tab_nieodwiedzonych[i] = i;
    }

    std::vector<int> currentPath;
    std::vector<int> temp_tab_nieodwiedzonych = tab_nieodwiedzonych;
    int score = 0;

    for (int j = 0; j < V; j++) {
        std::uniform_int_distribution<> dist(0, temp_tab_nieodwiedzonych.size() - 1);
        int randIndex = dist(g);

        int currentNode = temp_tab_nieodwiedzonych[randIndex];
        currentPath.push_back(currentNode);

        if (j > 0) {
            int prevNode = currentPath[j - 1];
            int weight = graph[prevNode][currentNode];

            // Sprawdzamy, czy istnieje połączenie
            if (weight == -1) {
                // Brak połączenia - przechodzimy do następnej iteracji
                score = INT_MAX;
                break;
            }
            score += weight;
        }

        temp_tab_nieodwiedzonych.erase(temp_tab_nieodwiedzonych.begin() + randIndex);
    }

    if (score != INT_MAX) {
        // Łączymy ostatni wierzchołek z pierwszym, aby zamknąć cykl
        int lastNode = currentPath.back();
        int firstNode = currentPath.front();
        int lastEdgeWeight = graph[lastNode][firstNode];

        if (lastEdgeWeight != -1) {  // Jeśli połączenie istnieje
            score += lastEdgeWeight;
            currentPath.push_back(firstNode);  // Zamyka cykl

            if (score < lowestCost) {
                lowestCost= score;
                bestPath = currentPath;

//                    // Jeśli znaleziono rozwiązanie równe temu z pliku, kończymy algorytm
//                    if (shortestpath == shortest_path_from_file) {
//                        //if_ended_by_iterations = false;
//                        return shortestpath;
//                    }
            }
        }
    }
}

bool AlgorytmyZad3::ifAllVisited(std::vector<bool> odwiedzone) {//metoda pomocniczna do NN
    for(int i = 0 ; i < odwiedzone.size() ; i++) {
        if (odwiedzone[i] == false) {
            return false;
        }
    }
    return true;

}
std::vector<int> AlgorytmyZad3::getBestPath() {
    return bestPath;
}
int AlgorytmyZad3::getLowestCost() {
    return lowestCost;
}

int AlgorytmyZad3::swapMethod(std::vector<std::vector<int>> &graph, int V, std::vector<int> &temp, int i, int j) {
    if(i == 0) {
        temp[temp.size() - 1] = temp[j];
    }
    std::swap(temp[i], temp[j]);

    int cost = 0;
    for(int k = 0 ; k < V - 1 ; k++) {//zliczanie dlugosci nowej sciezki
        cost += graph[temp[k]][temp[k+1]];
    }
    cost += graph[temp[V-1]][temp[0]];//koszt powrotu do wierz startowego
    return cost;
}

int AlgorytmyZad3::twoOpt(std::vector<std::vector<int>> &graph, int V, std::vector<int> &temp, int i, int j) {
    if(i == 0) {//jesli zamienieamy pierwszy to musimy tez ostatni (do ostatniego nie dochodzi petla)
        temp[temp.size() - 1] = temp[j];
    }
    std::reverse(temp.begin() + i, temp.begin() + j + 1);//zamienia kolejnosc [od;do)

    int cost = 0;
    for(int k = 0 ; k < V - 1 ; k++) {//zliczanie dlugosci nowej sciezki
        cost += graph[temp[k]][temp[k+1]];
    }
    cost += graph[temp[V-1]][temp[0]];//koszt powrotu do wierz startowego
    return cost;
}

bool AlgorytmyZad3::ifOptimumFound() {
    if(solutionFromFile == -1 || solutionFromFile != lowestCost)
        return false;
    else
        return true;
}

bool AlgorytmyZad3::ifInProcentageOfLowerBound(int LB){
    if (LB == -1)//nie przerywamy bo nie mamy nawet podstaw na to
        return false;
    else{
        auto LC = static_cast<double>(lowestCost);
        auto lower_bound = static_cast<double>(LB);
        if (100 * ((LC - lower_bound)/lower_bound)<= procentageOfLowerBound)
            return true;
        else
            return false;
    }
}



int AlgorytmyZad3::countAbsoluteError() {//blad bezwzgledny
    if(solutionFromFile != -1)
        return lowestCost - solutionFromFile;
    else
        return -1;
}

double AlgorytmyZad3::countRelativeError() {
    if(solutionFromFile != -1)
        return (static_cast<double>(lowestCost) - static_cast<double>(solutionFromFile))/static_cast<double>(solutionFromFile);
    else
        return -1.0;
}

int AlgorytmyZad3::returnRandom(int size) {
    return std::rand() % size;
}

std::vector<int> AlgorytmyZad3::swap(std::vector<int> old_path, int V) {
    int a = returnRandom(V);
    int b = returnRandom(V);
    while (a == b) {//zapewnienie ze a!=b
        a = returnRandom(V);
    }

    if(a == 0) {
        old_path[old_path.size() - 1] = old_path[b];
    }
    if(b == 0) {
        old_path[old_path.size() - 1] = old_path[a];
    }
    std::swap(old_path[a], old_path[b]);
    return old_path;
}

std::vector<int> AlgorytmyZad3::betterTwoOpt(std::vector<int> old_path, int V) {
    int a = returnRandom(V);
    int b = returnRandom(V);
    while (a == b) {//zapewnienie ze a!=b
        a = returnRandom(V);
    }

    if(a == 0) {//jesli zamienieamy pierwszy to musimy tez ostatni (do ostatniego nie dochodzi petla)
        old_path[old_path.size() - 1] = old_path[b];
    }
    if(b == 0) {//jesli zamienieamy pierwszy to musimy tez ostatni (do ostatniego nie dochodzi petla)
        old_path[old_path.size() - 1] = old_path[a];
    }
    if(a < b)
        std::reverse(old_path.begin() + a, old_path.begin() + b + 1);//zamienia kolejnosc [od;do)
    else
        std::reverse(old_path.begin() + b, old_path.begin() + a + 1);
    return(old_path);
}

int AlgorytmyZad3::countingPath(std::vector<int> path, std::vector<std::vector<int> > &graph, int V) {
    int cost = 0;
    for(int k = 0 ; k < V - 1 ; k++) {//zliczanie dlugosci nowej sciezki
        cost += graph[path[k]][path[k+1]];
    }
    cost += graph[path[V-1]][path[0]];//koszt powrotu do wierz startowego
    return cost;
}

int AlgorytmyZad3::Prim(std::vector<std::vector<int>> &graph) {
    int n = graph.size(); // Liczba wierzchołków
    std::vector<bool> visited(n, false); // Czy wierzchołek został odwiedzony
    std::vector<int> min_edge(n, INT_MAX); // Minimalny koszt dotarcia do wierzchołka
    min_edge[0] = 0; // Startujemy od wierzchołka 0

    int mst_cost = 0; // Koszt MST

    for (int i = 0; i < n; ++i) {
        int u = -1;
        for (int j = 0; j < n; ++j) {
            if (!visited[j] && (u == -1 || min_edge[j] < min_edge[u])) {
                u = j;
            }
        }

        if (min_edge[u] == INT_MAX) {
            std::cout << "Graf jest niespójny!" << std::endl;
            return -1;
        }

        visited[u] = true; // Oznacz wierzchołek jako odwiedzony
        mst_cost += min_edge[u]; // Dodaj koszt do MST

        // Zaktualizuj minimalne koszty dla sąsiadów
        for (int v = 0; v < n; ++v) {
            if (graph[u][v] != 0 && !visited[v]) { // Jeśli istnieje krawędź i wierzchołek nieodwiedzony
                min_edge[v] = std::min(min_edge[v], graph[u][v]);
            }
        }
    }

    return mst_cost;
}





