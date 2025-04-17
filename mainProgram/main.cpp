#include <iostream>

#include "Program.h"

int main()
{
    std::cout << "Program started!" << std::endl;
    Program mainProgram;
    mainProgram.glownyProgram();
    system("pause"); // waits for user input
    return 0;
}
