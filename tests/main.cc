#include "../source/matek.h"

#include <iostream>
#include <cmath>
#include <iomanip>

constexpr size_t PRECISION = 32;
static size_t called_for = 0;

static inline real_t modulo(real_t x, real_t y) {
    return static_cast<int>(x) % static_cast<int>(y);
}

void* operator new(const size_t size) {
    called_for++;
    return malloc(size);
}

int main(void) {
    std::string expression;

    Operators.add({ '^', OperatorPrecedence::HIGH , std::pow });
    Operators.add({ '%', OperatorPrecedence::HIGH , modulo   });

    while (true) {
        std::cout << ">> ";
        std::getline(std::cin, expression);

        if (expression == "exit" || expression == "q") {
            std::cout << "New called for " << called_for << " Times\n";
            return 0;
        }

        Matek matek;

        matek.setprecision(PRECISION);
        matek.expression(expression);


        real_t value = matek.evaluate();

        matek.printast();
        std::cout << std::fixed << std::setprecision(PRECISION) << value << std::endl;
    }

    return EXIT_SUCCESS;
}
