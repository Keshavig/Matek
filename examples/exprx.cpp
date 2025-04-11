/* A mathematical expressioin evaluater */

#include <iostream>
#include <cmath>
#include <iomanip>

#include "../source/matek.h"

constexpr size_t PRECISION = 5;
static size_t called_for = 0;

using namespace Matek;

void* operator new(const size_t size) {
    called_for++;
    return malloc(size);
}

static Matek::BinaryOperators Operators = {
    /*         FUNCTION                        PRECEDENCE            NAMES       */
    { [](real_t x, real_t y) { return x+y; }, Precedence::LOW,  "+", "plus",  "add" },
    { [](real_t x, real_t y) { return x-y; }, Precedence::LOW,  "-", "minus", "sub" },
    { [](real_t x, real_t y) { return x*y; }, Precedence::HIGH, "*", "times", "mul" },
    { [](real_t x, real_t y) { return x/y; }, Precedence::HIGH, "/", "div",  "quot" },
    };

void handleError(const Real& rl) {
    switch (rl.errorKind) {
        case Errorkind::ERROR_INVALID_OPERATOR: std::cerr << "Error: Invalid operator as found\n"; break;
        case Errorkind::ERROR_EMPTY_EXPRESSION: std::cerr << "Warning: Empty Expressionn\n"; break;
        case Errorkind::ERROR_UNEQUAL_PARENS:   std::cerr << "Error: Syntax Error, you have missing closing or opening parenthesis\n"; break;
        case Errorkind::ERROR_INVALID_SYNTAX:   std::cerr << "Error: Invalid Syntax\n"; break;
        case Errorkind::ERROR_INVALID_PAREN_SYNTAX: std::cerr << "Error: Invalid Parenthesis syntax\n"; break;
        case Errorkind::ERROR_I_DONT_KNOW: std::cerr << "Error: I dont know what is wrong but something definitely is\n"; break;
        default: break;
    };
}


int main(void) {
    Maysin math(Operators);
    std::string expression;

    Real answer = {};

    while(true)
    {
        std::cout << "[Expression]> ";
        std::getline(std::cin, expression);

        if (expression == "cls" || expression == "clear") {
            std::cout << "\033[2J\033[1;1H";
            continue;
        } else if (expression == "ast") {
            math.printast();
            continue;
        } else if (expression == "new") {
            std::cout << "new: " << called_for << '\n';
        } else if (expression == "exit" || expression == "q") break;

        math.setprecision(PRECISION);
        answer = math.evaluate(expression.c_str());

        if (answer.errorKind != Errorkind::ERROR_NONE) {
            handleError(answer);
            continue;
        }

        std::cout << std::fixed << std::setprecision(PRECISION) << answer.value << std::endl;
    }

    // std::cout << "New called for " << called_for << " Times\n";
    return EXIT_SUCCESS;
}
