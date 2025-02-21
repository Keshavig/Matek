#pragma once

#include <vector>
#include <initializer_list>

#include "tokenType.h"

typedef double (*functionPtr) (const double val1, const double val2);

/* Make operatorSymbol, operatorFunction and tokenType all static const and makae it 
 * work with removeAt and remove methods */
struct singleOperator {
    char  operatorSymbol;
    functionPtr operatorFunction;
    TokenType tokenType = TokenType::OPERATOR;

    singleOperator(const singleOperator& so) = default;
    singleOperator(const char os, const functionPtr of): operatorSymbol(os), operatorFunction(of) {}

    ~singleOperator() = default;

};

typedef std::vector<singleOperator> operatorList;

class BasicOperators {
    public:
        BasicOperators(const operatorList& basicOperators);
        BasicOperators(std::initializer_list<singleOperator> initList) : m_OperatorList(initList) {}

        /* Append functions */
        void adlist(const operatorList& OperatorList);
        void adone(const singleOperator& singleOperator);

        /* Removing functions */
        void overwrite(const operatorList& newOperatorList);
        void removeAt(const size_t position);
        void remove(const char character);

        /* Get data function */
        singleOperator get(const size_t position) const;
        size_t size(void) const;

    private:
        operatorList m_OperatorList;
};
