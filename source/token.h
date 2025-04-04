/* This file just defines/declares important stuff that other files use */
#pragma once

#include <string>

enum class TokenType {
    OPERATOR = 0,
    NUMBER,
    LPAREN,
    RPAREN,
    END,
    VARIABLE,
    FUNCTION,
};

struct Token {
    const char* tokenSymbol;
    TokenType tokenType;

    Token() = delete;
    Token(const Token& tkn) = default;

    Token(const char* ts, const TokenType tt) : tokenSymbol(ts), tokenType(tt) {}
};
