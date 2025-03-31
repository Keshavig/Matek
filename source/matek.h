#pragma once

#include "node.h"
#include "operators.h"

#define EXIT_FAILURE 1

class Matek final {
public:
    Matek(const BinaryOperators& Operators) : m_Operators(Operators) {}
    Matek(const Matek& mtk) = delete;

    void expression(const std::string& expr);
    void disableChecks(void);
    std::unique_ptr<BaseAst> parse();

    bool setprecision(size_t precision);

    void printast(void) const;
    real_t evaluate();

private:
    const BinaryOperators& m_Operators;

    std::string m_expression;
    std::unique_ptr<BaseAst> m_ast;

    bool m_docheck = true;
    size_t m_Precision = DEFAULT_PRECISION;

    real_t eval(const std::unique_ptr<BaseAst>& ast);
};
