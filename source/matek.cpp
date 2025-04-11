#include "matek.h"
#include "checks.h"
#include "error.h"
#include "node.h"
#include "operators.h"
#include "parser.h"
#include "real.h"

using namespace Matek;

Real Maysin::setExpression(const std::string_view expr) {
    if (expr.empty()) return {ERRCODE, Errorkind::ERROR_EMPTY_EXPRESSION, {}};
    m_expression = expr;

    if (!m_docheck) return {0, Errorkind::ERROR_NONE, {}};

    Checks c(expr);
    bool retval1 = c.checkParenCount();
    bool retval2 = c.checkParenSyntax();

    if (!retval1) return {ERRCODE, Errorkind::ERROR_UNEQUAL_PARENS, {}};
    if (!retval2) return {ERRCODE, Errorkind::ERROR_INVALID_PAREN_SYNTAX, {}};

    return {};
}

/* TOOD: Check for errors */
Real Maysin::eval(const std::unique_ptr<Matek::BaseAst>& node) {
    if (auto numberNode = dynamic_cast<Matek::NumberNode*>(node.get())) {
        return {numberNode->value, Errorkind::ERROR_NONE, {}};
    }

    else if (auto binaryNode = dynamic_cast<Matek::BinaryNode*>(node.get())) {
        Matek::real_t leftValue  = Maysin::eval(std::move(binaryNode->leftNode)).value;
        Matek::real_t rightValue = Maysin::eval(std::move(binaryNode->rightNode)).value;

        size_t len = m_operators.length();

        /* TODO: We should not need to run this loop since we already run this same one in @lexer.cpp 
         * We should just store the loops result somewhere */

        for (size_t i = 0; i < len; ++i) {
            const Matek::singleOperator so = m_operators.get(i);
            if (so.isvalidSymbol(binaryNode->Operator)) {
                return { so.m_function(leftValue, rightValue), Errorkind::ERROR_NONE, {}};
            }
        }
    }

    return {0, Errorkind::ERROR_I_DONT_KNOW, {}};
}

void Maysin::printast(void) const {
    if (!m_ast) return;

    m_ast->print(m_Precision);
    std::cout << '\n';
}

Real Maysin::evaluate(const std::string_view expression) {
    Real _ = setExpression(expression);
    if (_.errorKind != Errorkind::ERROR_NONE) return _;

    ourParser.updateExpression(m_expression.data());

    try { m_ast = ourParser.parse(); }
    catch (const std::runtime_error& e) {
        return {ERRCODE, m_currentError, {}};
    }

    return eval(m_ast);
}

void Maysin::disableChecks(void) {
    m_docheck = false;
}

bool Maysin::setprecision(size_t precision = DEFAULT_PRECISION) {
    // remove magic number
    if (precision > 32) return false;

    m_Precision = precision;
    return true;
}
