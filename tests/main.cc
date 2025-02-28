#include "../source/matek.h"

#include <iostream>
#include <iomanip>

constexpr size_t PRECISION = 10;

int main(void) {
    std::string expression;

    while (true) {
        std::cout << "$ ";
        std::getline(std::cin, expression);

        if (expression == "exit" || expression == "q") {
            return 0;
        }

        Matek matek;

        matek.expression(expression);
        matek.setprecision(PRECISION);

        real_t value = matek.evaluate();
        std::cout << "---------------------------------------------------------------------\n\n";
        std::cout << std::fixed << std::setprecision(PRECISION) << value << std::endl;
        std::cout << "\n\n---------------------------------------------------------------------\n            AST\n";

        matek.printast();
    }

    return 0;
}
