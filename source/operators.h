#pragma once

#include <vector>
#include <optional>
#include <initializer_list>

#include "token.h"


enum OperatorPrecedence {
    HIGH = 0,
    LOW  = 1,
};

using real_t = long double;
typedef real_t (*functionPtr) (const real_t val1, const real_t val2);

struct singleOperator {
    char  operatorSymbol;

    inline real_t eval(real_t a, real_t b) const;
    inline OperatorPrecedence getPrecedence(void) const;

    singleOperator(const singleOperator& so) = default;
    singleOperator(const char os, const OperatorPrecedence precedence, const functionPtr of)
        : operatorSymbol(os), m_operatorFunction(of), m_Precedence(precedence) {}

private:
    functionPtr m_operatorFunction;
    OperatorPrecedence  m_Precedence;

};

using operatorList = std::vector<singleOperator>;

class BasicOperators {
public:
    BasicOperators(const operatorList& basicOperators);
    BasicOperators(std::initializer_list<singleOperator> initList) : m_OperatorList(initList) {}

    /* Append functions */
    void addlist(const operatorList& OperatorList);
    void add(const singleOperator& singleOperator);

    /* Removing functions */
    void overwrite(const operatorList& newOperatorList);
    void removeAt(const size_t position);
    void remove(const char character);

    /* Get data function */
    singleOperator get(const size_t position) const;
    size_t size(void) const;

    std::optional<real_t> runfunc(const char _operator, real_t arg1, real_t arg2);

private:
    operatorList m_OperatorList;
};
