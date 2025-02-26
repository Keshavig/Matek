#include "../source/matek.h"

#include <iostream>

int main(void) {
    std::string expression;

    std::cout << "Welcome here [ Hero ] " << std::endl;
    std::cout << "$ ";
    std::getline(std::cin, expression);

    Matek matek;

    matek.expression(expression);
    real_t value = matek.evaluate();

    matek.printast();
    fprintf(stdout, "%.3Lf\n", value);

    return 0;
}
