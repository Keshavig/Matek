#pragma once
#include "parser.h"

#define EXIT_FAILURE 1

/* Defined in file lexer.cc */
extern BasicOperators Operators;

class Matek final {
public:
    Matek(void) = default;
    Matek(const Matek& mtk) = delete;

    void expression(const std::string_view expr);
    void disableChecks(void);
    std::unique_ptr<BaseAst> parse();

    bool setprecision(size_t precision);

    void printast(void) const;
    real_t evaluate();

private:
    real_t privateEval(const std::unique_ptr<BaseAst>& ast);
    bool m_docheck = true;
    
    size_t m_Precision = DEFAULT_PRECISION;

    std::string_view m_expression;
    std::unique_ptr<BaseAst> m_ast;
};
