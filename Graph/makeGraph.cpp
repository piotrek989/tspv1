//
// Created by Piotr on 10.12.2024.
//

#include "makeGraph.h"


makeGraph::makeGraph()
{

}

void makeGraph::getFromFile(std::string nazwaPlikuWe, std::vector<std::vector<int>>& graph, int& V, int& solutionFromFile) {
    std::ifstream inputFile(nazwaPlikuWe);

    if (!inputFile.is_open()) {
        throw std::runtime_error("Nie udało się otworzyć pliku: " + nazwaPlikuWe);
    }

    // Wczytanie liczby wierzchołków
    inputFile >> V;
    if (inputFile.fail()) {
        throw std::runtime_error("Błąd podczas odczytu liczby wierzchołków z pliku.");
    }

    // Wczytanie najlepszego znalezionego wyniku
    inputFile >> solutionFromFile;
    if (inputFile.fail()) {
        throw std::runtime_error("Błąd podczas odczytu wartości shortest_path z pliku.");
    }

    // Wczytanie macierzy sąsiedztwa
    graph.resize(V, std::vector<int>(V));
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            inputFile >> graph[i][j];
            if (inputFile.fail()) {
                throw std::runtime_error("Błąd podczas odczytu macierzy sąsiedztwa z pliku.");
            }
        }
    }

    inputFile.close();
}




void makeGraph::generateGraph(std::vector<std::vector<int>>& graph, int density, bool isDirected, int V){
    // Ustawienie grafu jako pustej macierzy sąsiedztwa
    graph.clear();
    graph.resize(V, std::vector<int>(V, -1));

    // Obliczenie liczby krawędzi na podstawie gęstości
    int maxEdges = isDirected ? V * (V - 1) : V * (V - 1) / 2;
    int targetEdges = (maxEdges * density) / 100;

    // Zmienna licznikowa dla liczby krawędzi
    int edgesAdded = 0;

    // Losowanie krawędzi
    srand(static_cast<unsigned int>(time(nullptr)));

    while (edgesAdded < targetEdges) {
        int u = rand() % V; // Losowy wierzchołek początkowy
        int v = rand() % V; // Losowy wierzchołek końcowy

        // Sprawdzamy, czy nie tworzymy pętli własnej lub duplikatów
        int edgeWeight = rand() % 1000 + 1;
        if (u != v && graph[u][v] == -1) {

            graph[u][v] = edgeWeight; // Dodajemy krawędź
            edgesAdded++;

            if (!isDirected) {
                graph[v][u] = edgeWeight; // Dodajemy krawędź w obu kierunkach, jeśli graf nieskierowany
            }
        }
    }

    // // Opcjonalne wyświetlenie wygenerowanego grafu (do debugowania)
    // std::cout << "Generated graph (adjacency matrix):\n";
    // for (const auto& row : graph) {
    //     for (int val : row) {
    //         std::cout << val << " ";
    //     }
    //     std::cout << "\n";
    // }
}

