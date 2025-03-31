#include "../source/matek.h"

#include <iostream>
#include <cmath>
#include <iomanip>

constexpr size_t PRECISION = 5;
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

    Operators.add({ std::pow, OperatorPrecedence::High, "^", "pow" });
    Operators.add({ modulo,   OperatorPrecedence::High, "%", "mod" });

    Matek matek;

    while (true) {
        std::cout << "Expression: ";
        std::getline(std::cin, expression);

        if (expression == "exit" || expression == "q") {
            std::cout << "New called for " << called_for << " Times\n";
            return 0;
        }

        matek.setprecision(PRECISION);
        matek.expression(expression);

        real_t value = matek.evaluate();

        // matek.printast();
        std::cout << std::fixed << std::setprecision(PRECISION) << value << std::endl;
    }

    std::cout << "New called for " << called_for << " Times\n";
    return EXIT_SUCCESS;
}
