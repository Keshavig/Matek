/* A mathematical expressioin evaluater */

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

static Matek::BinaryOperators Operators = {
    /*         FUNCTION                        PRECEDENCE            NAMES       */
    { [](real_t x, real_t y) { return x+y; }, Precedence::Low,  "+", "plus",  "add" },
    { [](real_t x, real_t y) { return x-y; }, Precedence::Low,  "-", "minus", "sub" },
    { [](real_t x, real_t y) { return x*y; }, Precedence::High, "*", "times", "mul" },
    { [](real_t x, real_t y) { return x/y; }, Precedence::High, "/", "div",  "quot" },
};

int main(void) {
    Monster math = Operators;
    std::string expression;

    while (true) {
        std::cout << "[Expression]> ";
        std::getline(std::cin, expression);

        if (expression == "ast") {
            math.printast();
            continue;
        }

        if (expression == "exit" || expression == "q") break;

        math.setprecision(PRECISION);
        std::cout << std::fixed << std::setprecision(PRECISION) << math.evaluate(expression.c_str()) << std::endl;
    }

    std::cout << "New called for " << called_for << " Times\n";
    return EXIT_SUCCESS;
}
