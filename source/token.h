/* This file just defines/declares important stuff that other files use */
#pragma once

#include <string>

enum class TokenType {
    OPERATOR = 0,
    NUMBER,
    VARIABLE,
    FUNCTION,
    LPAREN,
    RPAREN,
    NEWSYMBOL,
    END,
    INVALID,
};

struct Token {
    std::string tokenSymbol;
    TokenType tokenType;

    Token() = delete;
    Token(const Token& tkn) = default;
    Token(const std::string& ts, const TokenType tt) : tokenSymbol(ts), tokenType(tt) {}
};
