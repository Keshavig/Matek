#pragma once

#include <string>

/* This file just defines/declares important stuff that other file later on use */

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

    Token() = default;
    Token(const Token& tkn) = default;
    Token(const std::string& ts, const TokenType tt) : tokenSymbol(ts), tokenType(tt) {}
};
