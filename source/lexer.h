#pragma once

#include "operators.h"
#include <string_view>

class Lexer {
private:
    const std::string_view m_expression;
    size_t currentTokenPosition;

public:
    Lexer(const std::string_view expression);
    Token getnextToken(void);
    size_t getindex(void) const;

private:
    bool isaspace(const char ch);
    bool isadigit(const char ch);
};
