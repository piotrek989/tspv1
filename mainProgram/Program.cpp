//
// Created by Piotr on 10.12.2024.
//

#include "Program.h"

#include "Timer.h"
#include "../Algorithms/AlgorytmyZad3.h"

Program::Program() {

}

bool stringToBool(const std::string& str) {
    if (str == "true") {
        return true;
    } else if (str == "false") {
        return false;
    }
    // Jeśli string nie jest ani "true" ani "false", możesz zwrócić domyślną wartość, np. false, albo rzucić wyjątek
    throw std::invalid_argument("Invalid input: not a valid boolean string.");
}

void Program::wczytanieZPlikuKonfiguracyjnego() {
    // Ścieżka do pliku konfiguracyjnego - do uzupełnienia
    const std::string sciezkaDoPliku = "../Files/plikKonfiguracyjny.txt"; // Podaj tutaj ścieżkę do pliku

    std::ifstream plik(sciezkaDoPliku);
    if (!plik.is_open()) {
        std::cerr << "Nie udało się otworzyć pliku konfiguracyjnego: " << sciezkaDoPliku << std::endl;
        return;
    }

    std::string linia;
    while (std::getline(plik, linia)) {
        // Znajdź pozycję dwukropka
        std::size_t pozycjaDwukropka = linia.find(':');
        if (pozycjaDwukropka == std::string::npos) {
            std::cerr << "Niepoprawny format linii w pliku konfiguracyjnym: " << linia << std::endl;
            continue;
        }

        // Klucz to wszystko przed dwukropkiem (włącznie z nim)
        std::string klucz = linia.substr(0, pozycjaDwukropka + 1);
        // Wartość to wszystko po dwukropku
        std::string wartosc = linia.substr(pozycjaDwukropka + 1);

        // Usuń ewentualne białe znaki z wartości (np. spacje na początku)
        wartosc.erase(0, wartosc.find_first_not_of(" \t\r\n"));

        // Dodaj do mapy
        konfiguracja[klucz] = wartosc;

    }


    //uwaga w kluczu podajemy z dwukropkiem
    maxCzasAlgorytmow = std::stoi(konfiguracja["max czas wykonywania algorytmu[ms]:"]);
    iteracjeAlgorytmow = std::stoi(konfiguracja["liczba powtorzen algorytmow:"]);
    nazwaPlikuWejsciowego = konfiguracja["nazwa pliku wejsciowego:"];
    nazwaPlikuWyjsciowego = konfiguracja["nazwa pliku wyjsciowego:"];
    ifFromFile = stringToBool(konfiguracja["test z pojedynczego pliku:"]);
    ifGenerateInitSolutionWithNn = stringToBool(konfiguracja["generowanie poczatkowego rozwiazania nn:"]);
    ifGenerateNeighbourhoodWithSwap = stringToBool(konfiguracja["generowanie sasiedztwa swapem:"]);
    T_min = std::stof(konfiguracja["T_min:"]);
    T_max = std::stof(konfiguracja["T_max:"]);
    alfa = std::stof(konfiguracja["alfa:"]);
    kadencja = std::stoi(konfiguracja["kadencja:"]);
    wielkoscListyTabu = std::stoi(konfiguracja["dlugosc listy tabu:"]);
    plik.close();
}


void Program::glownyProgram() {
    wczytanieZPlikuKonfiguracyjnego();
    std::cout << "max czas wykonywania algorytmu[ms]: " << maxCzasAlgorytmow << "\n";
    std::cout << "liczba powtorzen algorytmow: " << iteracjeAlgorytmow << "\n";
    std::cout<<"Nazwa pliku we: "<<nazwaPlikuWejsciowego<<std::endl;
    std::cout<<"Nazwa pliku wy: "<<nazwaPlikuWyjsciowego<<std::endl;
    std::cout << "Generowac pocz rozw nn?: "<<ifGenerateInitSolutionWithNn<<std::endl;
    std::cout << "Generowac sasiedztwo swapem?: "<<ifGenerateNeighbourhoodWithSwap<<std::endl;
    std::cout << "T_minimalna: " << T_min << std::endl;
    std::cout << "T_maksymalna: " << T_max << std::endl;
    std::cout << "Alfa: " << alfa << std::endl;
    std::cout << "Kadencja: " << kadencja << std::endl;
    std::cout << "Wielkosc listy tabu: " << wielkoscListyTabu << std::endl;


    std::vector<std::vector<int>> graph;
    int V = 10;

    int i = 0;
    int korzyscSimANnealing = 0;
    int korzyscTaboo = 0;
    int takiesame = 0;
    while(i < 50) {
        makeGraph make_graph;//graph/getosc/czyskierowany(false --> inst. syme)/liczba wierzcholkow
        if (ifFromFile) {
            make_graph.getFromFile(nazwaPlikuWejsciowego, graph, V, solutionFromFile);
        }else {
            make_graph.generateGraph(graph, 100, false, V);
        }

        //std::cout<<std::endl;

        Timer timer(maxCzasAlgorytmow);
//        AlgorytmyZad3 simulated_annealing;
//        timer.startCounter();
//        simulated_annealing.SAlgorithm(graph, V, T_max, T_min, alfa);
//        double t1 = timer.getCounter();
//        std::cout<<std::endl;
//        std::cout<<"Wyrzazanie: "<<t1<<"ms, najnizszy koszt: "<<simulated_annealing.getLowestCost()<<std::endl;
//        for(int i = 0 ; i < simulated_annealing.getBestPath().size() ; i++) {
//            std::cout<<simulated_annealing.getBestPath()[i]<<" ";
//        }
//        std::cout<<std::endl;

        AlgorytmyZad3 tabuSearch(ifGenerateInitSolutionWithNn, ifGenerateNeighbourhoodWithSwap);
        timer.startCounter();
        tabuSearch.TS(graph, V, 50, 50);
        double t2 = timer.getCounter();

        std::cout<<"Taboo search: "<<t2<<"ms, najnizszy koszt: "<<tabuSearch.getLowestCost()<<std::endl;
//        for(int i = 0 ; i < tabuSearch.getBestPath().size() ; i++) {
//            std::cout<<tabuSearch.getBestPath()[i]<<" ";
//        }
        std::cout<<std::endl;

        AlgorytmyZad3 tabuSearch1(ifGenerateInitSolutionWithNn, !ifGenerateNeighbourhoodWithSwap);
        timer.startCounter();
        tabuSearch1.TS(graph, V, 50, 50);
        double t3 = timer.getCounter();

        std::cout<<"Taboo search1: "<<t3<<"ms, najnizszy koszt: "<<tabuSearch1.getLowestCost()<<std::endl;
//        for(int i = 0 ; i < tabuSearch1.getBestPath().size() ; i++) {
//            std::cout<<tabuSearch1.getBestPath()[i]<<" ";
//        }
        std::cout<<std::endl;
//
//        if(simulated_annealing.getLowestCost() > tabuSearch.getLowestCost())
//            korzyscTaboo++;
//        else if (simulated_annealing.getLowestCost() < tabuSearch.getLowestCost())
//            korzyscSimANnealing++;
//        else if (simulated_annealing.getLowestCost() == tabuSearch.getLowestCost()) {
//            takiesame++;
//        }

        i++;
    }
    std::cout<<"Najmniejsze koszty ile razy:"<<std::endl;
    std::cout<<"SA: "<<korzyscSimANnealing<<std::endl;
    std::cout<<"TS: "<<korzyscTaboo<<std::endl;
    std::cout<<"Ile razy takie same: "<<takiesame<<std::endl;

}

