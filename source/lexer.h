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

        char* storage;
        Errorkind* m_error;

    public:
        Lexer() = delete;
        Lexer(const Lexer& l) = delete;
        Lexer(const BinaryOperators& Operators, const std::string_view expression, Errorkind* error);

        Token getnextToken(void);
        size_t getindex(void) const;
        void updateExpression(const char* expr);

        ~Lexer();
        int gntcallc;
        int m_index;
};
}
