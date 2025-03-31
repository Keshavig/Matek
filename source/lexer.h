#pragma once

#include "operators.h"

class Lexer {
private:
    const std::string m_expression;
    const BinaryOperators& m_Operators;
    const size_t m_operatorsLength;

    int m_index;

public:
    Lexer() = delete;
    Lexer(const Lexer& l) = delete;
    Lexer(const BinaryOperators& Operators, const std::string& expression);

    Token getnextToken(void);
    size_t getindex(void) const;

private:
    std::string collectSymbol(void);
};
