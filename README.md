# **Travelling Salesman Problem 1**

**TSP** is a classic optimization problem where  
the goal is to find the shortest possible route that visits each city **exactly once**  
and returns to the **starting point**.

In this repo, you can see the TSP solved using the following methods:
- **Repetitive Nearest Neighbour**
- **Random Approach**
- **Tabu Search**
- **Simulated Annealing**

The PDF named **Raport_2.pdf** contains:
- **Time results**
- **Correctness evaluations**
- **Graphical representations** of outcomes
- **Comparison** between all implemented methods  
  (Algorithms from both the first and second parts of the project are included.)

---

## **How to Run the Project**

1. **Build the project using Clion and CMake**:

    ```bash
    cmake -B cmake-build-debug
    cmake --build cmake-build-debug
    ```

2. **Navigate to the output folder**:

    ```bash
    cd cmake-build-debug
    ```

3. **Run the program**:

    ```bash
    ./zadanie_3.exe
    ```

Or in Windows command line (CMD), from the main project directory:

1. **Go into cmake-build-debug**
```cmd
 cd cmake-build-debug
```
2. **Run exe**
```cmd
   .\zadanie_3.exe
```
