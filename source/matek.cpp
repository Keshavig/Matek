#include "matek.h"
#include "checks.h"
#include "operators.h"
#include "parser.h"

void Matek::expression(const std::string& expr) {
    m_expression = expr;

    if (m_docheck)
    {
        Checks check(expr);
        bool retval1 = check.checkParenCount();
        bool retval2 = check.checkParenSyntax();

        if (!retval1) {
            std::cerr << COLOR_RED << "ERROR: Unequal number of opening and closing parenthesis" << RESET_TERM_COLOR << '\n';
            exit(EXIT_FAILURE);
        }

        if (!retval2) {
            std::cerr << COLOR_RED << "ERROR: Invalid parenthesis's"  << RESET_TERM_COLOR << '\n';
            exit(EXIT_FAILURE);
        }
    }
}

real_t Matek::eval(const std::unique_ptr<BaseAst>& node) {
    if (auto numberNode = dynamic_cast<NumberNode*>(node.get())) {
        return numberNode->value;
    }

    else if (auto binaryNode = dynamic_cast<BinaryNode*>(node.get())) {
        real_t left  = Matek::eval(std::move(binaryNode->leftNode));
        real_t right = Matek::eval(std::move(binaryNode->rightNode));

        size_t len = m_Operators.length();
        for (size_t i = 0; i < len; ++i) {
            singleOperator currentOperator = m_Operators.get(i);
            if (currentOperator.isvalidSymbol(binaryNode->Operator)) return currentOperator.m_function(left, right);
        }
    }

    std::cerr << COLOR_RED << "ERROR: Invalid ast" << RESET_TERM_COLOR << '\n';
    exit(EXIT_FAILURE);
}

void Matek::printast(void) const {
    if (!m_ast) return;

    m_ast->print(m_Precision);
    std::cout << '\n';
}

real_t Matek::evaluate() {
    Parser parser(m_Operators, m_expression);
    m_ast = parser.parse();

    return eval(m_ast);
}

void Matek::disableChecks(void) {
    m_docheck = false;
}

bool Matek::setprecision(size_t precision = DEFAULT_PRECISION) {
    // remove magic number
    if (precision > 32) {
        return false;
    }

    m_Precision = precision;
    return true;
}
