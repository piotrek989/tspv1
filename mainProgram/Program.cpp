//
// Created by Piotr on 10.12.2024.
//

#include "Program.h"

#include "Timer.h"
#include "../Algorithms/AlgorytmyZad3.h"

Program::Program()//lista inicjalizacyjna
    :solutionFromFile(-1),
     ifTesting(false)//przed zajeciam to zmienic na false
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
    std::cout << "Ery w SA: " << eras << std::endl;
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
    eras = std::stoi(konfiguracja["epoki w SA:"]);
    procentageOfLowerBound = std::stod(konfiguracja["procent dolnego ograniczenia:"]);
    plik.close();
}


void Program::glownyProgram() {
    wczytanieZPlikuKonfiguracyjnego();
    printConfigFile();

    std::vector<std::vector<int>> graph;//nasze dwie glowne zmienne
    int V;

    // ////
    // makeGraph makegraph;
    // makegraph.getFromFile(nazwaPlikuWejsciowego, graph, V, solutionFromFile);
    // int LB = AlgorytmyZad3::Prim(graph);
    // std::cout<<"LB: "<<LB<<std::endl;

    if (ifTesting) {//jesli to sa testy
        int z = 0;
        float all = 84;
        maxCzasAlgorytmow = 900000;//w[ms]
        potworzeniaAlgorytmow = 5;
        eras = 1000;//DO EW POPRAWY DO TESTOW
        iterationsWithoutImprove = 200;
        procentageOfLowerBound = 2;
        T_min = 0.00001;
        T_max = 10000.0;
        std::vector<double> times;
        double avgTime;
        double absErrorAvg;
        double relativeErrorAvg;
        std::vector<int> absoluteErrors;
        std::vector<double> relativeErrors;
        std::vector<int> solutions;
        std::vector<std::string> listaInstancji = {"5s","6s", "7s","8s", "9s", "10s", "11s", "12s","13s",
                                                    "5a","6a", "7a","8a", "9a", "10a", "11a", "12a","13a",
                                                    "17a", "34a", "65a", "100a", "171a",
                                                    "14s", "29s","100s","137s", "150s", "200s"};
        std::vector<double> alfas = {0.99, 0.995, 0.999};
        std::vector<bool> ifGeoCooling = {true, false};
        std::vector<bool> ifInitNN = {false, true};
        std::vector<bool> ifGenerateSwap = {true, false};
        std::vector<int> wielkosciListyTabu = {20, 50, 100};
        std::vector<int> kadencje = {10, 50, 100};
        std::vector<double> temperatures_max = {1000.0, 10000.0, 100000.0};
        std::vector<double> temperatures_max_geo = {15.0, 20.0, 30.0};


        makeGraph makegraph;
        Timer timer(maxCzasAlgorytmow);//moj obiekt zegara
        Timer timer2(maxCzasAlgorytmow);

        for(int a = 0 ; a < ifInitNN.size() ; a++) {
            ifGenerateInitSolutionWithNn = ifInitNN[a];
            if(ifGenerateInitSolutionWithNn) {
                listaInstancji = {"5s","6s", "7s","8s", "9s", "10s", "11s", "12s","13s",
                                                        "5a","6a", "7a","8a", "9a", "10a", "11a", "12a","13a",
                                                        "17a", "34a", "65a", "100a", "171a",
                                                        "14s", "29s","100s","137s", "150s", "200s"};
            }
            for(int b = 0 ; b < ifGenerateSwap.size() ; b++) {
                ifGenerateNeighbourhoodWithSwap = ifGenerateSwap[b];
                ifGeometricCooling = true;
                for(int c = 0 ; c < alfas.size();c++) {
                    alfa = alfas[c];
                    for(int d = 0  ; d < temperatures_max.size();d++){
                        T_max = temperatures_max[d];
                        makeGraph::writeInfo1(ifGenerateInitSolutionWithNn, ifGenerateNeighbourhoodWithSwap, ifGeometricCooling, alfa, T_max);
                        makeGraph::writeInitInfoForExcel();
                        for (int i = 0 ; i < listaInstancji.size(); i++) {
                            avgTime = 0.0;
                            absErrorAvg = 0.0;
                            relativeErrorAvg = 0.0;
                            std::string nazwaPojInstancji = "../Files/file_" + listaInstancji[i] + ".txt";//pelna nazwa badanej instacji wrazze sciezka
                            makegraph.getFromFile(nazwaPojInstancji, graph, V, solutionFromFile);
                            if(V <= 10) {//dla wiekszych grafow
                                procentageOfLowerBound = 0;
                            } else if(V > 10 && V <= 20)
                                procentageOfLowerBound = 10;
                            else
                                procentageOfLowerBound = 20;
                            int j;
                            timer2.startCounter();
                            for(j = 0 ; j < potworzeniaAlgorytmow ; j++) {
                                if(timer2.getCounter() > maxCzasAlgorytmow)
                                    break;
                                AlgorytmyZad3 simulated_annealing(timer, ifGenerateInitSolutionWithNn, ifGenerateNeighbourhoodWithSwap,
                                                                  ifGeometricCooling, eras, solutionFromFile, iterationsWithoutImprove, procentageOfLowerBound);
                                timer.startCounter();
                                simulated_annealing.SAlgorithm(graph, V, T_max, T_min, alfa);
                                double t1 = timer.getCounter();
                                times.push_back(t1);
                                avgTime += t1;
                                int lowestCost = simulated_annealing.getLowestCost();
                                solutions.push_back(lowestCost);

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
                            makeGraph::writeToFileTimesAndAvg(times, absoluteErrors, relativeErrors, solutions, avgTime, absErrorAvg, relativeErrorAvg);//wpisuje do pliku output1.txt
                            makeGraph::writeMainInfoForExcel(V, avgTime, absErrorAvg, relativeErrorAvg);
                            //czyszczenie
                            times.clear();
                            absoluteErrors.clear();
                            relativeErrors.clear();
                            solutions.clear();
                        }
                        z++;//36opcji
                        std::cout<<"Postep: "<<(static_cast<double>(z)/all)*100.0<<"/100%"<<std::endl;
                    }
                }
                for(int e = 0 ; e <  temperatures_max_geo.size();e++) {
                    T_max = temperatures_max_geo[e];
                    ifGeometricCooling = false;//chlodzenie logarytmiczne
                    T_min = 0.1;
                    makeGraph::writeInfo2(ifGenerateInitSolutionWithNn, ifGenerateNeighbourhoodWithSwap, ifGeometricCooling, T_max);
                    makeGraph::writeInitInfoForExcel();
                    for (int i = 0 ; i < listaInstancji.size(); i++) {

                        avgTime = 0.0;
                        absErrorAvg = 0.0;
                        relativeErrorAvg = 0.0;

                        std::string nazwaPojInstancji = "../Files/file_" + listaInstancji[i] + ".txt";//pelna nazwa badanej instacji wrazze sciezka
                        makegraph.getFromFile(nazwaPojInstancji, graph, V, solutionFromFile);
                        if(V <= 10) {//dla wiekszych grafow
                            procentageOfLowerBound = 2;
                        } else if(V > 10 && V <= 13)
                            procentageOfLowerBound = 5;
                        else
                            procentageOfLowerBound = 10;
                        // makeGraph::writeInitInfo(nazwaPojInstancji, "SA", solutionFromFile);
                        // makeGraph.printGraph(graph);
                        int j;
                        timer2.startCounter();
                        for(j = 0 ; j < potworzeniaAlgorytmow ; j++) {
                            if(timer2.getCounter() > maxCzasAlgorytmow)
                                break;
                            AlgorytmyZad3 simulated_annealing(timer, ifGenerateInitSolutionWithNn, ifGenerateNeighbourhoodWithSwap,
                                                              ifGeometricCooling, eras, solutionFromFile, iterationsWithoutImprove, procentageOfLowerBound);
                            timer.startCounter();
                            simulated_annealing.SAlgorithm(graph, V, T_max, T_min, alfa);
                            double t1 = timer.getCounter();
                            times.push_back(t1);
                            avgTime += t1;
                            int lowestCost = simulated_annealing.getLowestCost();
                            solutions.push_back(lowestCost);

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
                        makeGraph::writeToFileTimesAndAvg(times, absoluteErrors, relativeErrors, solutions, avgTime, absErrorAvg, relativeErrorAvg);//wpisuje do pliku output1.txt
                        makeGraph::writeMainInfoForExcel(V, avgTime, absErrorAvg, relativeErrorAvg);
                        //czyszczenie
                        times.clear();
                        absoluteErrors.clear();
                        relativeErrors.clear();
                        solutions.clear();
                    }
                    z++;//4opcje
                    std::cout<<"Postep: "<<(static_cast<double>(z)/all)*100.0<<"/100%"<<std::endl;
                }

                for(int c = 0 ; c < wielkosciListyTabu.size() ; c++) {
                    wielkoscListyTabu = wielkosciListyTabu[c];
                    for(int d = 0 ; d < kadencje.size() ; d++) {
                        kadencja = kadencje[d];
                        makeGraph::writeInfo3(ifGenerateInitSolutionWithNn, ifGenerateNeighbourhoodWithSwap, wielkoscListyTabu, kadencja);
                        makeGraph::writeInitInfoForExcel();
                        for (int i = 0 ; i < listaInstancji.size(); i++) {
                            avgTime = 0.0;
                            absErrorAvg = 0.0;
                            relativeErrorAvg = 0.0;

                            std::string nazwaPojInstancji = "../Files/file_" + listaInstancji[i] + ".txt";//pelna nazwa badanej instacji wrazze sciezka
                            makegraph.getFromFile(nazwaPojInstancji, graph, V, solutionFromFile);
                            if(V <= 10) {//dla wiekszych grafow
                                procentageOfLowerBound = 2;
                            } else if(V > 10 && V <= 13)
                                procentageOfLowerBound = 5;
                            else
                                procentageOfLowerBound = 10;
                            // makeGraph::writeInitInfo(nazwaPojInstancji, "SA", solutionFromFile);
                            // makeGraph.printGraph(graph);
                            int j;
                            timer2.startCounter();
                            for(j = 0 ; j < potworzeniaAlgorytmow ; j++) {
                                if(timer2.getCounter() > maxCzasAlgorytmow)
                                    break;
                                AlgorytmyZad3 tabuSearch(timer, ifGenerateInitSolutionWithNn, ifGenerateNeighbourhoodWithSwap,
                                                                  ifGeometricCooling, eras, solutionFromFile, iterationsWithoutImprove, procentageOfLowerBound);
                                timer.startCounter();
                                tabuSearch.TS(graph, V, wielkoscListyTabu, kadencja);
                                double t1 = timer.getCounter();
                                times.push_back(t1);
                                avgTime += t1;
                                int lowestCost = tabuSearch.getLowestCost();
                                solutions.push_back(lowestCost);

                                int absoluteError = tabuSearch.countAbsoluteError();
                                double relError = tabuSearch.countRelativeError();
                                absErrorAvg += absoluteError;
                                relativeErrorAvg += relError;
                                absoluteErrors.push_back(absoluteError);
                                relativeErrors.push_back(relError);
                            }
                            avgTime /= j;
                            relativeErrorAvg /= j;
                            absErrorAvg /= j;
                            makeGraph::writeToFileTimesAndAvg(times, absoluteErrors, relativeErrors, solutions, avgTime, absErrorAvg, relativeErrorAvg);//wpisuje do pliku output1.txt
                            makeGraph::writeMainInfoForExcel(V, avgTime, absErrorAvg, relativeErrorAvg);
                            //czyszczenie
                            times.clear();
                            absoluteErrors.clear();
                            relativeErrors.clear();
                            solutions.clear();
                        }
                        z++;//36 opcje
                        std::cout<<"Postep: "<<(static_cast<double>(z)/all)*100.0<<"/100%"<<std::endl;
                    }
                }

            }
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
                                              ifGeometricCooling, eras, solutionFromFile, iterationsWithoutImprove, procentageOfLowerBound);
            timer.startCounter();
            simulated_annealing.SAlgorithm(graph, V, T_max, T_min, alfa);
            double t1 = timer.getCounter();
            std::cout<<std::endl;
            std::cout<<"Wyrzazanie: "<<t1<<"ms, najnizszy koszt: "<<simulated_annealing.getLowestCost()<<std::endl;
            std::cout<<"Moja sciezka: ";
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
                                     eras, solutionFromFile, iterationsWithoutImprove,procentageOfLowerBound);

            timer.startCounter();
            tabuSearch.TS(graph, V, wielkoscListyTabu, kadencja);
            double t2 = timer.getCounter();

            std::cout<<"Taboo search: "<<t2<<"ms, najnizszy koszt: "<<tabuSearch.getLowestCost()<<std::endl;
            std::cout<<"Moja sciezka: ";
            for(int j = 0 ; j < tabuSearch.getBestPath().size() ; j++) {
                std::cout<<tabuSearch.getBestPath()[j]<<" ";
            }
            absError = tabuSearch.countAbsoluteError();
            relError = tabuSearch.countRelativeError();
            std::cout<<std::endl;
            std::cout<<"Bezwzgledny blad: "<<absError<<std::endl;
            std::cout<<"Wzgledny blad: "<<relError<<std::endl;
            std::cout<<"Wzgledny w [%]: "<<relError * 100.0<<std::endl;
            std::cout<<std::endl;

            i++;
        }
    }
}

