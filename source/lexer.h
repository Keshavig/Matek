#pragma once

#include <string>

#include "token.h"
#include "operators.h"

class Lexer {
    private:
        const std::string& m_expression;
        size_t currentTokenPosition;

    public:
        Lexer(const std::string& expression) : m_expression(expression), currentTokenPosition(0) {}
        Token getnextToken(void);

        size_t getindex(void) const;

    private:
        bool isaspace(const char ch);
        bool isadigit(const char ch);
};
