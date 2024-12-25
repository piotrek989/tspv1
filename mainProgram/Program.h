//
// Created by Piotr on 10.12.2024.
//

#ifndef PROGRAM_H
#define PROGRAM_H

#include<string>
#include<unordered_map>
#include<vector>
#include <fstream>
#include<iostream>

#include "../Graph/makeGraph.h"

class Program {
    int iteracjeAlgorytmow;
    int maxCzasAlgorytmow;
    int solutionFromFile;
    bool ifGenerateInitSolutionWithNn;
    bool ifGenerateNeighbourhoodWithSwap;
    bool ifGeometricCooling;
    int iterationsWithoutImprove;
    int iterationsToTakeWorse;
    float procentageOfLowerBound;
    float T_min;
    float T_max;
    float alfa;
    int kadencja;
    int wielkoscListyTabu;

    std::string nazwaPlikuWejsciowego;
    std::string nazwaPlikuWyjsciowego;
    bool ifFromFile;

    std::unordered_map<std::string, std::string> konfiguracja;//wczytywanie z pliku konfiguracyjnego

public:
    Program();
    void wczytanieZPlikuKonfiguracyjnego();
    void glownyProgram();


};



#endif //PROGRAM_H
