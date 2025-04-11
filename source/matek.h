#pragma once

#include "operators.h"
#include "parser.h"
#include "lexer.h"

#define EXIT_FAILURE 1

/* TODO: Initialize Lexer here so we don't have to initialize it everytime we run evaluate(...) */

class Maysin final {
    public:
        Maysin (const Matek::BinaryOperators& Operators) :
            m_operators(Operators), ourParser(Operators, m_expression, &m_currentError) {}

        Maysin (const Maysin& mtk) = delete;

        void disableChecks(void);
        std::unique_ptr<Matek::BaseAst> parse();

        bool setprecision(size_t precision);

        void printast(void) const;
        Matek::Real evaluate(const std::string_view expression);

        ~Maysin() = default;

    private:
        Matek::Real eval(const std::unique_ptr<Matek::BaseAst>& ast);
        Matek::Real setExpression(const std::string_view expr);

        Errorkind m_currentError;

    private:
        const Matek::BinaryOperators& m_operators;

        std::string_view m_expression;
        std::unique_ptr<Matek::BaseAst> m_ast;

        bool m_docheck = true;
        size_t m_Precision = DEFAULT_PRECISION;

        Matek::Parser ourParser;
};
