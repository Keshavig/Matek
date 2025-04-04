#include <iostream>
#include <iomanip>
#include "../source/matek.h"

const Matek::BinaryOperators Operators = {
    {[](real_t a, real_t b) { return a+b; }, Precedence::Low, "+"},
    {[](real_t a, real_t b) { return a-b; }, Precedence::Low, "-"},
    {[](real_t a, real_t b) { return a*b; }, Precedence::High, "*"},
    {[](real_t a, real_t b) { return a/b; }, Precedence::High, "/"},
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        return 1;
    }

    Monster math(Operators);
    const char* expression = argv[1];
    const real_t result = math.evaluate(expression);

    std::cout << std::fixed << std::setprecision(1) << result << std::endl;
    return 0;
}
