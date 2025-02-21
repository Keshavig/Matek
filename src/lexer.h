#pragma once

#include <string>

#include "tokenType.h"

struct Token {
    std::string tokenSymbol;
    TokenType tokenType;

    Token() = default;
    Token(const Token& tkn) = default;
    Token(const std::string& ts, const TokenType tt) : tokenSymbol(ts), tokenType(tt) {}
};

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
