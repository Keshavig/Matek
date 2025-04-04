#include "matek.h"
#include "checks.h"
#include "node.h"
#include "operators.h"
#include "parser.h"

bool Monster::setExpression(const std::string_view expr) {
    if (expr.empty()) {
        std::cerr << "Empty expression\n";
        return false;
    }

    /* TODO: Dont make `Checks` a class, just functions */
    if (m_docheck) {
        Checks check(expr);
        bool retval1 = check.checkParenCount();
        bool retval2 = check.checkParenSyntax();

        if (!retval1) {
            std::cerr << "ERROR: Unequal number of opening and closing parenthesis\n" ;
            exit(EXIT_FAILURE);
        }

        if (!retval2) {
            std::cerr << "ERROR: Invalid parenthesis syntax\n";
            exit(EXIT_FAILURE);
        }
    }

    m_expression = expr;
    return true;
}

real_t Monster::eval(const std::unique_ptr<Matek::BaseAst>& node) {
    if (auto numberNode = dynamic_cast<Matek::NumberNode*>(node.get())) {
        return numberNode->value;
    }

    else if (auto binaryNode = dynamic_cast<Matek::BinaryNode*>(node.get())) {
        real_t left  = Monster::eval(std::move(binaryNode->leftNode));
        real_t right = Monster::eval(std::move(binaryNode->rightNode));

        size_t len = m_operators.length();

        /* TODO: We should not need to run this loop since we already run this same one in @lexer.cpp 
         * We should just store the loops result somewhere */
        for (size_t i = 0; i < len; ++i) {
            const Matek::singleOperator so = m_operators.get(i);
            if (so.isvalidSymbol(binaryNode->Operator)) {
                return so.m_function(left, right);
            }

            /* NOTE: Here we dont need to print a error for saying `invalid operator` as that will already be checked by the lexer */
        }
    }

    std::cerr << "ERROR: Invalid ast" << '\n';
    return EXIT_FAILURE;
}

void Monster::printast(void) const {
    if (!m_ast) return;

    m_ast->print(m_Precision);
    std::cout << '\n';
}

real_t Monster::evaluate(const std::string_view expression) {
    /* TODO: Send anything other than 0 I think */
    if (!setExpression(expression)) return 0;
    ourParser.updateExpression(m_expression.data());
    m_ast = ourParser.parse();

    return eval(m_ast);
}

void Monster::disableChecks(void) {
    m_docheck = false;
}
bool Monster::setprecision(size_t precision = DEFAULT_PRECISION) {
    // remove magic number
    if (precision > 32) {
        return false;
    }

    m_Precision = precision;
    return true;
}

