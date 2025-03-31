#pragma once

#include <unordered_set>
#include <iostream>
#include <vector>

#include "token.h"

enum OperatorPrecedence {
    None = -1,
    High = 0,
    Low = 1,
};

using real_t = long double;
typedef real_t (*functionPtr) (const real_t val1, const real_t val2);

struct singleOperator {
private:
    std::unordered_set<std::string> m_operatorNames;

public:
    functionPtr m_function;
    OperatorPrecedence  m_precedence;

    bool isvalidSymbol(const std::string& osym) const;
    std::unordered_set<std::string> getSymbols(void) const;

    singleOperator(const singleOperator& so) = default;

    template <typename... Argpack>
    singleOperator(const functionPtr fp, const OperatorPrecedence precedence, Argpack... operatorNames) :
        m_operatorNames({ operatorNames...}),
        m_function(fp),
        m_precedence(precedence)
    {
        if (m_operatorNames.empty())
            std::cerr << "WARNING: argument pack is empty, this will cause given operator to be ignored\n";
    }
};

class BinaryOperators {
public:
    BinaryOperators(const std::vector<singleOperator>& basicOperators);
    BinaryOperators(std::initializer_list<singleOperator> initList) : m_OperatorList(initList) {}

    /* Append functions */
    void add(const std::vector<singleOperator>& OperatorList);
    void add(const singleOperator& singleOperator);

    /* Get data function */
    singleOperator get(const size_t position) const;
    singleOperator get(const std::string& Operator) const;

    OperatorPrecedence getPrecedence(const std::string& Operator) const;
    size_t length(void) const;

private:
    std::vector<singleOperator> m_OperatorList;
    void updateSymbols(const std::unordered_set<std::string>& unordSet);
};
