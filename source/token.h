#pragma once

#include <string>

enum class TokenType {
    OPERATOR,
    NUMBER,
    LPAREN,
    RPAREN,
    END,
    VARIABLE,
    FUNCTION,
    INVALID,
};

struct Token {
    const char* tokenSymbol;
    TokenType tokenType;

    Token() : tokenSymbol(nullptr), tokenType(TokenType::INVALID) {};
    Token(const Token& tkn) = default;

    Token(const char* ts, const TokenType tt) : tokenSymbol(ts), tokenType(tt) {}
};
