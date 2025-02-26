#pragma once

#include "parser.h"
#include "node.h"

class Matek final {
public:
    Matek(void) = default;
    Matek(const Matek& mtk) = delete;

    void expression(const std::string& expr);
    void disableChecks(void);
    std::unique_ptr<BaseAst> parse();

    void printast(void) const;
    real_t evaluate();

private:
    real_t privateEval(const std::unique_ptr<BaseAst>& ast);
    bool docheck = true;

    std::string m_expression;
    std::unique_ptr<BaseAst> m_ast;
};
