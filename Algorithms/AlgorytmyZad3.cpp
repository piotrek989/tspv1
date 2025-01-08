//
// Created by Piotr on 11.12.2024.
//

#include "AlgorytmyZad3.h"

#include <climits>

//uwaga dla grafu nieskierowanego alg dziala dobrze bo swapow jest n*(n-1)/2 ale w nieskierowanyn bedzie n*(n-1) ---> do uwaględnienia
AlgorytmyZad3::AlgorytmyZad3(Timer& timer, bool ifStartWithNN, bool ifGenerateWithSwap, bool ifgeometriColling, int iterationstotakewrse, int solFromFile, int iterwithoutimprove,double procOfLowerBoud)
    :timer_(timer),
    generateInitSolutionWithNn(ifStartWithNN),
    ifGenerateNeighbourhoodWithSwap(ifGenerateWithSwap),
    ifGeometricCooling(ifgeometriColling),
    iterationsToTakeWorse(iterationstotakewrse),
    solutionFromFile(solFromFile),
    iterationsWithoutImprove(iterwithoutimprove),
    procentageOfLowerBound(procOfLowerBoud)
    {}


void AlgorytmyZad3::SAlgorithm(std::vector<std::vector<int>>& graph, int V, double T_max, double T_min, double alfa) {
    int l = 0;
    std::srand(static_cast<unsigned>(time(0))); // Seed dla generatora liczb pseudolosowych
    if(generateInitSolutionWithNn) {
        repetetiveNearestNeighbour(graph, V);//ustawia mi pola klasy lowest cost i wpisuje najlepsza sciezke
    }else{
        randomMethod(graph, V);
    }
    int helper = lowestCost;//inicjalizacja helpera

    double T;
    int k = 1;//epoka
    if(ifGeometricCooling) {
        T = T_max;//inicjacja poczatkowej temperatury
    }else{
        T = T_max/log(1.0 + static_cast<double>(k));
    }

    // printCostnPath();

    std::vector<int> pom;//potrzebne do kolejnych generowan nowych sasiadow

    int x_a = lowestCost;
    std::vector<int> path_a = bestPath;

    while (T > T_min && !ifOptimumFound() && !ifInProcentageOfLowerBound()) {
        helper = lowestCost;

        double elapsed_time = timer_.getCounter();
        if (elapsed_time >= timer_.time_limit) {
            return;
        }
        if (k >= iterationsWithoutImprove)
            return;

        if (!ifGeometricCooling)//tylko jesli chlodzenie logarytmiczne
            T = T_max/log(1.0 + static_cast<double>(k));

        if(generateInitSolutionWithNn)
            pom = path_a;//to bylo normalnie
        else
            pom = bestPath;


        for(int i = 0 ; i < V - 1; i++) {//odpowiada za generowanie wszystkich kombinacji
            for(int j = i + 1; j < V ; j++) {
                std::vector<int> tmp_path = pom;
                int tmp_cost;
                if(ifGenerateNeighbourhoodWithSwap) {//w zaleznosci od tej zmiennej generujemy sasiedztwo
                    tmp_cost = swapMethod(graph, V, tmp_path, i, j);//metoda swapujaca -- metoda jest tylko dobrze dzialajaca dla grafu pełnego
                }else{
                    tmp_cost = twoOpt(graph, V, tmp_path, i , j);
                }

                if(tmp_cost < lowestCost){//poprawianie najlepszego z najlepszych
                    // helper = lowestCost;//pomocnik gdy utkienimy w randomie
                    lowestCost = tmp_cost;
                    bestPath = tmp_path;
                }
                if (tmp_cost < x_a) {//jesli obecnie rozpatrywanae od teraz wygenerowanego(tmp_cost) jest gorsze do poprawa obecnie rozpatrywanego
                    x_a = tmp_cost;//x_a to najlepsze wygenerowane w sasiedztwie
                    path_a = tmp_path;
                   } else {
                    auto cost_new = static_cast<double>(tmp_cost);
                    auto cost_old = static_cast<double>(lowestCost);
                    double p = 1.0/(1.0 + std::exp((cost_old - cost_new)/T));
                    auto r = static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX);
                        if(r < p) {
                            x_a = tmp_cost;//to bylo zakomentowane
                            path_a = tmp_path;
                        }
                }
            }
        }
        if (lowestCost == helper)
            l++;

        if(!generateInitSolutionWithNn && lowestCost == helper && l > 5) {
            int hlp = lowestCost;
            std::vector<int> road = bestPath;
            randomMethod(graph, V);
            pom = bestPath;
            lowestCost = hlp;
            bestPath = road;
            l = 0;
        }

        if (ifGeometricCooling)//tylko jesli chlodzenie geo
            T *= alfa;
        k++;
    }
    //printCostnPath();
}

void AlgorytmyZad3::TS(std::vector<std::vector<int>>& graph, int V, int sizeOfTabuList, int kandencja) {
    if(generateInitSolutionWithNn) {
        repetetiveNearestNeighbour(graph, V);//ustawia mi pola klasy lowest cost i wpisuje najlepsza sciezke
    }else{
        randomMethod(graph, V);
    }


    // printCostnPath();//meotda dodatkowa printuje mi wyniki dla mnie;

    std::vector<std::pair<std::vector<int>, int>> listaTabu;

    int x_a = lowestCost;
    std::vector<int> path_a = bestPath;

    std::vector<int> pom;

    int i = 0;
    int k = 1;
    while (i < iterationsWithoutImprove && !ifOptimumFound() && !ifInProcentageOfLowerBound()) {
        double elapsed_time = timer_.getCounter();
        if (elapsed_time >= timer_.time_limit) {
            return;
        }

        std::pair<std::vector<int>, int> x;
        x.first = path_a;
        x.second = kandencja;
        listaTabu.push_back(x);//dodanie do listy tabu nowego rozwiazania
        pom = path_a;
        for(int i = 0 ; i < V - 1 ; i++) {//odpowiada za generowanie wszystkich kombinacji
            for(int j = i + 1; j < V ; j++) {
                std::vector<int> tmp_path = pom;
                int tmp_cost;
                if(ifGenerateNeighbourhoodWithSwap) {//w zaleznosci od tej zmiennej generujemy sasiedztwo
                    tmp_cost = swapMethod(graph, V, tmp_path, i, j);//metoda swapujaca -- metoda jest tylko dobrze dzialajaca dla grafu pełnego
                }else{
                    tmp_cost = twoOpt(graph, V, tmp_path, i , j);
                }

                bool isOnTabooList = ifInTabuList(listaTabu, tmp_path);//zmienna przechowuje mi czy dane rozw jest na taboo liscie

                if ((tmp_cost < x_a && !isOnTabooList) || (isOnTabooList && tmp_cost < lowestCost)) {//tmp_cost to kandydat na x_a, x_a to aktualne rozpatrywane\\\ten drugi warunek to kryt aspiracji
                    if(tmp_cost < lowestCost) {
                        lowestCost = tmp_cost;
                        bestPath = tmp_path;//zmiana dotychczasowego naj rozwiazania
                        k = 0;//jesli poprawiono dotychczasowe naj rozwiazanie to licznik bez poprawy zerujemy
                    }
                    x_a = tmp_cost;
                    path_a = tmp_path;
                }
                else {
                    if (!isOnTabooList && k == iterationsToTakeWorse) {//gdy nie jest na liscie tabu
                        x_a = tmp_cost;//te dwie zmienne przechowuja najlepsze z najlepszych koszty
                        path_a = tmp_path;
                        k = 0;//reset zmiennej k
                    }
                }

            }

        }
        if (listaTabu.size() > sizeOfTabuList) listaTabu.erase(listaTabu.begin());
//        std::cout<< listaTabu.size()<<std::endl;
        decrementCadency(listaTabu);
        k++;//tutaj zwiekszamy gdy nie ma poprawy
        i++;
    }

//    printCostnPath();

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


int AlgorytmyZad3::repetetiveNearestNeighbour(std::vector<std::vector<int>>& graph, int V) {
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

void AlgorytmyZad3::randomMethod(std::vector<std::vector<int>> &graph, int V) {
    std::random_device rd;  // rd jest wykorzystywane do generowania losowej wartości (ziarno)
    std::mt19937 g(rd());   // tworze generator liczb pseudolosowych, inicjalizowany wartością z urządzenia losowego

    lowestCost = INT_MAX;//poczatkowa wartosc lowestCost
    std::vector<int> tab_nieodwiedzonych(V);

    for (int i = 0; i < V; i++) {  // indeksów tyle, ile wierzchołków
        tab_nieodwiedzonych[i] = i;
    }

    std::vector<int> currentPath;  // Ścieżka dla bieżącej iteracji
    std::vector<int> temp_tab_nieodwiedzonych = tab_nieodwiedzonych;  // Tymczasowa lista wierzchołków do odwiedzenia
    int score = 0;

    for (int j = 0; j < V; j++) {
        std::uniform_int_distribution<> dist(0, temp_tab_nieodwiedzonych.size() - 1);  // losowanie indeksu
        int randIndex = dist(g);  // generowanie liczby z distribution liczb z zadanego wyżej przedziału

        int currentNode = temp_tab_nieodwiedzonych[randIndex];
        currentPath.push_back(currentNode);  // dodanie wierzchołka do ścieżki

        if (j > 0) {
            // Obliczamy wagę krawędzi między bieżącym a poprzednim wierzchołkiem
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

        // Usuwamy odwiedzony wierzchołek z listy
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
    if(i == 0) {//jesli zamienieamy pierwszy to musimy tez ostatni (do ostatniego nie dochodzi petla)
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

bool AlgorytmyZad3::ifInProcentageOfLowerBound(){
    if (solutionFromFile == -1)//nie przerywamy bo nie mamy nawet podstaw na to
        return false;
    else{
        auto LC = static_cast<double>(lowestCost);
        auto SFF = static_cast<double>(solutionFromFile);
        if (100 * ((LC - SFF)/SFF)<= procentageOfLowerBound)
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


