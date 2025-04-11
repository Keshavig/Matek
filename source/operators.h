#pragma once

#include <iostream>
#include <vector>
#include <unordered_set>
#include <string_view>

#include "token.h"
#include "real.h"

enum class Precedence {
    INVALID = -1,
    HIGH = 0,
    LOW = 1,
};

using symbolsList = std::unordered_set<std::string_view>;
typedef Matek::real_t (*functionPtr) (const Matek::real_t val1, const Matek::real_t val2);

namespace Matek {
    struct singleOperator {
        functionPtr m_function;
        Precedence  m_precedence;

        bool isvalidSymbol(const std::string_view osym) const;
        symbolsList getSymbols(void) const;

        singleOperator(const singleOperator& so) = default;

        template <typename... Argpack>
            singleOperator(const functionPtr fp, const Precedence precedence, Argpack... operatorNames) :
                m_operatorNames({ operatorNames...}), m_function(fp), m_precedence(precedence) {
                    if (m_operatorNames.empty())
                        std::cerr << "WARNING: argument pack is empty, operator will be ignored\n";
                }

        private:
            symbolsList m_operatorNames;
    };

    struct BinaryOperators {
        BinaryOperators(const std::vector<singleOperator>& basicOperators);
        BinaryOperators(std::initializer_list<singleOperator> initList);

        /* Append functions */
        void add(const std::vector<singleOperator>& OperatorList);
        void add(const singleOperator& singleOperator);

        /* Get data function */
        singleOperator get(const size_t position) const;
        singleOperator get(const char* Operator) const;

        Precedence getPrecedence(const char* Operator) const;
        size_t length(void) const;

        private:
            std::vector<singleOperator> m_OperatorList;
            void updateSymbols(const symbolsList& unordSet);
    };
};
