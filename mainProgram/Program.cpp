//
// Created by Piotr on 10.12.2024.
//

#include "Program.h"

#include "Timer.h"
#include "../Algorithms/AlgorytmyZad3.h"

Program::Program()//lista inicjalizacyjna
    :solutionFromFile(-1),
     ifTesting(true)//przed zajeciam to zmienic na false
{}


bool stringToBool(const std::string& str) {
    if (str == "true") {
        return true;
    } else if (str == "false") {
        return false;
    }
    // Jeśli string nie jest ani "true" ani "false", możesz zwrócić domyślną wartość, np. false, albo rzucić wyjątek
    throw std::invalid_argument("Invalid input: not a valid boolean string.");
}

void Program::printConfigFile() {
    std::cout << "max czas wykonywania algorytmu[ms]: " << maxCzasAlgorytmow << "\n";
    std::cout << "liczba powtorzen algorytmow: " << potworzeniaAlgorytmow << "\n";
    std::cout<<"Nazwa pliku we: "<<nazwaPlikuWejsciowego<<std::endl;
    std::cout<<"Nazwa pliku wy: "<<nazwaPlikuWyjsciowego<<std::endl;
    std::cout << "Generowac pocz rozw nn?: "<<ifGenerateInitSolutionWithNn<<std::endl;
    std::cout << "Generowac sasiedztwo swapem?: "<<ifGenerateNeighbourhoodWithSwap<<std::endl;
    std::cout << "T_minimalna: " << T_min << std::endl;
    std::cout << "T_maksymalna: " << T_max << std::endl;
    std::cout << "Procent dolnego ograniczenia: " << procentageOfLowerBound << std::endl;
    std::cout << "Alfa: " << alfa << std::endl;
    std::cout << "Czy chlodzenie geometryczne: " << ifGeometricCooling << std::endl;
    std::cout << "Kadencja: " << kadencja << std::endl;
    std::cout << "Wielkosc listy tabu: " << wielkoscListyTabu << std::endl;
    std::cout << "Ilosc iteracji bez poprawy: " << iterationsWithoutImprove << std::endl;
    std::cout << "Ilosc iteracji by wziac gorsze roz: " << iterationsToTakeWorse << std::endl;
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
    potworzeniaAlgorytmow = std::stoi(konfiguracja["liczba powtorzen algorytmow:"]);
    nazwaPlikuWejsciowego = konfiguracja["nazwa pliku wejsciowego:"];
    nazwaPlikuWyjsciowego = konfiguracja["nazwa pliku wyjsciowego:"];
    ifFromFile = stringToBool(konfiguracja["test z pojedynczego pliku:"]);
    ifGenerateInitSolutionWithNn = stringToBool(konfiguracja["generowanie poczatkowego rozwiazania nn:"]);
    ifGenerateNeighbourhoodWithSwap = stringToBool(konfiguracja["generowanie sasiedztwa swapem:"]);
    ifGeometricCooling = stringToBool(konfiguracja["chlodzenie geometryczne:"]);
    T_min = std::stod(konfiguracja["T_min:"]);
    T_max = std::stod(konfiguracja["T_max:"]);
    alfa = std::stod(konfiguracja["alfa:"]);
    kadencja = std::stoi(konfiguracja["kadencja:"]);
    wielkoscListyTabu = std::stoi(konfiguracja["dlugosc listy tabu:"]);
    iterationsWithoutImprove = std::stoi(konfiguracja["ilosc iteracji bez poprawy:"]);
    iterationsToTakeWorse = std::stoi(konfiguracja["ilosc iteracji by wziac gorsze rozw:"]);
    procentageOfLowerBound = std::stod(konfiguracja["procent dolnego ograniczenia:"]);
    plik.close();
}


void Program::glownyProgram() {
    wczytanieZPlikuKonfiguracyjnego();
    printConfigFile();

    std::vector<std::vector<int>> graph;//nasze dwie glowne zmienne
    int V;

    if (ifTesting) {//jesli to sa testy
        std::vector<double> times;
        double avgTime;
        double absErrorAvg;
        double relativeErrorAvg;
        std::vector<int> absoluteErrors;
        std::vector<double> relativeErrors;
        std::vector<int> solutions;
        std::vector<std::string> listaInstancji = {"10s", "13s","34a", "65a"};
        makeGraph makegraph;
        Timer timer(maxCzasAlgorytmow);//moj obiekt zegara
        Timer timer2(maxCzasAlgorytmow);

        for (int i = 0 ; i < listaInstancji.size(); i++) {
            avgTime = 0.0;
            absErrorAvg = 0.0;
            relativeErrorAvg = 0.0;

            std::string nazwaPojInstancji = "../Files/file_" + listaInstancji[i] + ".txt";
            std::cout<<"Wczytano plik o nazwie: "<<nazwaPojInstancji<<std::endl;
            makegraph.getFromFile(nazwaPojInstancji, graph, V, solutionFromFile);
            makeGraph::writeInitInfo(nazwaPojInstancji, "SA", solutionFromFile);
            // makeGraph.printGraph(graph);
            int j;
            timer2.startCounter();
            std::cout<<"Alg: SA"<<std::endl;
            for(j = 0 ; j < potworzeniaAlgorytmow ; j++) {
                if(timer2.getCounter() > maxCzasAlgorytmow)
                    break;
                AlgorytmyZad3 simulated_annealing(timer, ifGenerateInitSolutionWithNn, ifGenerateNeighbourhoodWithSwap,
                                                  ifGeometricCooling, iterationsToTakeWorse, solutionFromFile, iterationsWithoutImprove, procentageOfLowerBound);
                timer.startCounter();
                simulated_annealing.SAlgorithm(graph, V, T_max, T_min, alfa);
                double t1 = timer.getCounter();
                times.push_back(t1);
                avgTime += t1;
                int lowestCost = simulated_annealing.getLowestCost();
                solutions.push_back(lowestCost);
                std::cout<<j + 1<<". "<<t1<<"ms, koszt: "<<lowestCost<<std::endl;

                int absoluteError = simulated_annealing.countAbsoluteError();
                double relError = simulated_annealing.countRelativeError();
                absErrorAvg += absoluteError;
                relativeErrorAvg += relError;
                absoluteErrors.push_back(absoluteError);
                relativeErrors.push_back(relError);
            }
            avgTime /= j;
            relativeErrorAvg /= j;
            absErrorAvg /= j;
            makeGraph::writeToFileTimesAndAvg(times, absoluteErrors, relativeErrors, solutions, avgTime, absErrorAvg, relativeErrorAvg);
            //czyszczenie
            times.clear();
            absoluteErrors.clear();
            relativeErrors.clear();
            solutions.clear();
        }

    } else {//jesli to sa zajecia
        int i = 0;
        while(i < 1) {
            makeGraph make_graph;//graph/getosc/czyskierowany(false --> inst. syme)/liczba wierzcholkow
            isUnDirected = false;//domyślnie ustawiamy ze graf nie jest skierowany (tzn ze jest sym)
            if (ifFromFile) {
                make_graph.getFromFile(nazwaPlikuWejsciowego, graph, V, solutionFromFile);
                isUnDirected = make_graph.isSymetric(graph, V);//sprawdzamy czy graf symetryczny
                std::cout<<"Czy graf symetryczny: "<< isUnDirected << std::endl;
            }else {
                V = 10;
                make_graph.generateGraph(graph, 100, isUnDirected, V);//tutaj podajemy czy skierowany czy nie

            }

            std::cout<<"Rozwiazanie z pliku: "<<solutionFromFile << std::endl;


            //std::cout<<std::endl;

            Timer timer(maxCzasAlgorytmow);
            AlgorytmyZad3 simulated_annealing(timer, ifGenerateInitSolutionWithNn, ifGenerateNeighbourhoodWithSwap,
                                              ifGeometricCooling, iterationsToTakeWorse, solutionFromFile, iterationsWithoutImprove, procentageOfLowerBound);
            timer.startCounter();
            simulated_annealing.SAlgorithm(graph, V, T_max, T_min, alfa);
            double t1 = timer.getCounter();
            std::cout<<std::endl;
            std::cout<<"Wyrzazanie: "<<t1<<"ms, najnizszy koszt: "<<simulated_annealing.getLowestCost()<<std::endl;
            for(int j = 0 ; j < simulated_annealing.getBestPath().size() ; j++) {
                std::cout<<simulated_annealing.getBestPath()[j]<<" ";
            }
            int absError = simulated_annealing.countAbsoluteError();
            float relError = simulated_annealing.countRelativeError();
            std::cout<<std::endl;
            std::cout<<"Bezwzgledny blad: "<<absError<<std::endl;
            std::cout<<"Wzgledny blad: "<<relError<<std::endl;
            std::cout<<"Wzgledny w [%]: "<<relError * 100.0<<std::endl;
            std::cout<<std::endl;
            ////////////////////////////////////
            AlgorytmyZad3 tabuSearch(timer, ifGenerateInitSolutionWithNn, ifGenerateNeighbourhoodWithSwap, ifGeometricCooling,
                                     iterationsToTakeWorse, solutionFromFile, iterationsWithoutImprove,procentageOfLowerBound);

            timer.startCounter();
            tabuSearch.TS(graph, V, wielkoscListyTabu, kadencja);
            double t2 = timer.getCounter();

            std::cout<<"Taboo search: "<<t2<<"ms, najnizszy koszt: "<<tabuSearch.getLowestCost()<<std::endl;
            for(int j = 0 ; j < tabuSearch.getBestPath().size() ; j++) {
                std::cout<<tabuSearch.getBestPath()[j]<<" ";
            }

            std::cout<<std::endl;

            i++;
        }
    }
}

