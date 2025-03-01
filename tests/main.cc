#include "../source/matek.h"

#include <iostream>
#include <cmath>
#include <iomanip>

constexpr size_t PRECISION = 10;

static inline real_t modulo(real_t x, real_t y) {
    return static_cast<int>(x) % static_cast<int>(y);
}

int main(void) {
    std::string expression;

    Operators.add({ '^', OperatorPrecedence::HIGH , std::pow });
    Operators.add({ '%', OperatorPrecedence::HIGH , modulo   });

    while (true) {
        std::cout << "$ ";
        std::getline(std::cin, expression);

        if (expression == "exit" || expression == "q") {
            return 0;
        }

        Matek matek;

        matek.setprecision(PRECISION);
        matek.expression(expression);

        auto value = matek.evaluate();

        std::cout << std::fixed << std::setprecision(PRECISION) << value << std::endl;
        matek.printast();
    }

    return 0;
}
