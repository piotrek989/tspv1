cmake_minimum_required(VERSION 3.28)
project(zadanie_3)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_GENERATOR_PLATFORM x64) # Dla 64-bit
set(CMAKE_EXE_LINKER_FLAGS "-static -W")

add_executable(zadanie_3 mainProgram/main.cpp
        Graph/makeGraph.cpp
        Graph/makeGraph.h
        mainProgram/Program.cpp
        mainProgram/Program.h
        Algorithms/AlgorytmyZad3.cpp
        Algorithms/AlgorytmyZad3.h
        mainProgram/Timer.cpp
        mainProgram/Timer.h)