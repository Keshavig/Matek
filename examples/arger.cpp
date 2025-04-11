#include <iostream>
#include <iomanip>
#include "../source/matek.h"

using namespace Matek;
void handleError(const ErrorData& Erdata, const Errorkind erkind) {
    switch (erkind) {
        case Errorkind::ERROR_SUDDEN_END_OF_EXPRESSION: std::cerr << "ERROR: Invalid syntax\n"; break;
        case Errorkind::ERROR_DIVISION_BY_ZERO:         std::cerr << "Math Error: division by zero\n"; break;
        case Errorkind::ERROR_NUMBER_OUT_OF_RANGE:      std::cerr << "ERROR: The number is wayy to big\n"; break;
        case Errorkind::ERROR_INVALID_OPERATOR:         std::cerr << "Error: Invalid operator as found\n"; break;
        case Errorkind::ERROR_EMPTY_EXPRESSION:         std::cerr << "Warning: Empty Expressionn\n"; break;
        case Errorkind::ERROR_UNEQUAL_PARENS:           std::cerr << "Error: Syntax Error, you have missing closing or opening parenthesis\n"; break;
        case Errorkind::ERROR_INVALID_SYNTAX:           std::cerr << "Error: Invalid Syntax\n"; break;
        case Errorkind::ERROR_INVALID_PAREN_SYNTAX:     std::cerr << "Error: Invalid Parenthesis syntax\n"; break;
        case Errorkind::ERROR_EXPECTED_NUMBER:          std::cerr << "Error: Expected number\n"; break;
        case Errorkind::ERROR_I_DONT_KNOW:              std::cerr << "Error: I dont know what is wrong but something definitely is\n"; break;
        default: std::cerr << "How are we here?\n"; break;
    };
}

const Matek::BinaryOperators Operators = {
    {[](real_t a, real_t b) { return a+b; }, Precedence::LOW, "+"},
    {[](real_t a, real_t b) { return a-b; }, Precedence::LOW, "-"},
    {[](real_t a, real_t b) { return a*b; }, Precedence::HIGH, "*"},
    {[](real_t a, real_t b) { return a/b; }, Precedence::HIGH, "/"},
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        return 0;
    }

    Maysin math = Operators;

    std::string expression;
    for (size_t i = 1; i < argc; ++i) {
        expression += argv[i];
    }

    const Real result = math.evaluate(expression);

    if (result.errorKind != Errorkind::ERROR_NONE) {
        handleError(result.errorData, result.errorKind);
        std::cerr << "OK?\n";
        std::cout << std::fixed << std::setprecision(2) << result.value << std::endl;
        return 1;
    }

    std::cout << std::fixed << std::setprecision(2) << result.value << std::endl;
    return 0;
}
