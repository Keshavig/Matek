#pragma once

#include <cassert>
#include <cstdlib>
#include <optional>
#include <vector>
#include <string>
#include <iostream>

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
    std::vector<std::string> m_operatorNames;

public:
    real_t eval(real_t a, real_t b) const;
    OperatorPrecedence getPrecedence(void) const;

    /* Checks if osym is an element of vector args */
    bool isvalidOperatorSymbol(const std::string& osym) const;
    singleOperator(const singleOperator& so) = default;


    template <typename... Argpack>
    singleOperator(const functionPtr fp, const OperatorPrecedence precedence, Argpack... operatorNames) :
        m_operatorFunction(fp),
        m_Precedence(precedence)
    {
        /*  TODO: Idk how but if user enter anything other than string to the pack, give a "good" error message */
        const size_t length = sizeof...(operatorNames);
        m_operatorNames.reserve(length);

        m_operatorNames = { operatorNames... };

        // assert(!m_args.empty());
        if (m_operatorNames.empty()) {
            std::cerr << "WARNING: argument pack is empty, this will cause given operator to be ignored\n";
        }
    }

private:
    functionPtr m_operatorFunction;
    OperatorPrecedence  m_Precedence;

};

class BasicOperators {
public:
    BasicOperators(const std::vector<singleOperator>& basicOperators);
    BasicOperators(std::initializer_list<singleOperator> initList) : m_OperatorList(initList) {}

    /* Append functions */
    void addlist(const std::vector<singleOperator>& OperatorList);
    void add(const singleOperator& singleOperator);

    /* Removing functions */
    // void overwrite(const std::vector<singleOperator>& newOperatorList);
    // void removeAt(const size_t position);
    // void remove(const std::string_view character);

    /* Get data function */
    singleOperator get(const size_t position) const;
    singleOperator get(const std::string& Operator) const;

    size_t size(void) const;

    /* NOTE: Optionals are slow right? */
    std::optional<real_t> runfunc(const std::string& _operator, real_t arg1, real_t arg2);

private:
    std::vector<singleOperator> m_OperatorList;
};
