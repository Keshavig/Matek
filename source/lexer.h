#pragma once

#include "operators.h"
#include <string_view>

namespace Matek 
{
    class Lexer {
    private:
        std::string_view m_expression;
        const BinaryOperators& m_Operators;
        size_t m_exprlen;
        int m_index; // This is integer just for the sake of the constructor

        char* stringStorage;

    public:
        Lexer() = delete;
        Lexer(const Lexer& l) = delete;
        Lexer(const BinaryOperators& Operators, const std::string_view expression);

        Token getnextToken(void);
        size_t getindex(void) const;
        void updateExpression(const char* expr);

        ~Lexer();
};
}
