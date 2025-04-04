#pragma once

#include "operators.h"
#include "parser.h"
#include "lexer.h"

#define EXIT_FAILURE 1

/* TODO: Initialize Lexer here so we dont't have to initialize it everytime we run evaluate(...) */

class Monster final {
    public:
        Monster (const Matek::BinaryOperators& Operators) : m_operators(Operators), ourParser(Operators, m_expression) {}
        Monster (const Monster& mtk) = delete;

        void disableChecks(void);
        std::unique_ptr<Matek::BaseAst> parse();

        bool setprecision(size_t precision);

        void printast(void) const;
        real_t evaluate(const std::string_view expression);

        ~Monster() = default;

    private:
        real_t eval(const std::unique_ptr<Matek::BaseAst>& ast);
        bool setExpression(const std::string_view expr);

    private:
        const Matek::BinaryOperators& m_operators;

        std::string_view m_expression;
        std::unique_ptr<Matek::BaseAst> m_ast;

        bool m_docheck = true;
        size_t m_Precision = DEFAULT_PRECISION;

        Matek::Parser ourParser;
};
