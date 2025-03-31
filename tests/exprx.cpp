#include "../source/matek.h"

#include <iostream>
#include <cmath>
#include <iomanip>

constexpr size_t PRECISION = 3;
static size_t called_for = 0;

void* operator new(const size_t size) {
    called_for++;
    return malloc(size);
}

static BinaryOperators Operators = {
    { [](real_t x, real_t y) { return x+y; }, OperatorPrecedence::Low,  "+", "plus",  "add" },
    { [](real_t x, real_t y) { return x-y; }, OperatorPrecedence::Low,  "-", "minus", "sub" },
    { [](real_t x, real_t y) { return x*y; }, OperatorPrecedence::High, "*", "times", "mul" },
    { [](real_t x, real_t y) { return x/y; }, OperatorPrecedence::High, "/", "div",  "quot" },
};

int main(void) {
    Matek math = Operators;
    std::string expression;

    while (true)
    {
        std::cout << "Expression: ";
        std::getline(std::cin, expression);

        if (expression == "ast") {
            math.printast();
            continue;
        }

        if (expression == "exit" || expression == "q")
        {
            std::cout << "New called for " << called_for << " Times\n";
            return 0;
        }

        math.setprecision(PRECISION);
        math.expression(expression);

        real_t value = math.evaluate();
        std::cout << std::fixed << std::setprecision(PRECISION) << value << std::endl;

    }

    std::cout << "New called for " << called_for << " Times\n";
    return EXIT_SUCCESS;
}
