#include "matek.h"
#include "checks.h"

/* Checks are not properly working I think */
void Matek::expression(const std::string& expr) {
    m_expression = expr;
    if (docheck) {
        Checks check(expr);
        bool retval1 = check.checkParenCount();
        bool retval2 = check.checkParenSyntax();

        if (!retval1)
            fprintf(stderr, "error: please close your parenthesis\n");

        if (!retval2)
            fprintf(stderr, "error: invalid parenthesis syntax\n");
    }
}

real_t Matek::privateEval(const std::unique_ptr<BaseAst>& node) {
    if (auto numberNode = dynamic_cast<NumberNode*>(node.get())) {
        return numberNode->value;
    }

    else if (auto binaryNode = dynamic_cast<BinaryNode*>(node.get())) {
        real_t left  = Matek::privateEval(std::move(binaryNode->leftNode));
        real_t right = Matek::privateEval(std::move(binaryNode->rightNode));

        std::optional<real_t> retval = operatorsList.runfunc(binaryNode->Operator, left, right);

        if (!retval) {
            fprintf(stderr, "%serror: invalid operator '%c' %s\n", COLOR_RED, binaryNode->Operator, RESET_TERM_COLOR);
            exit(EXIT_FAILURE);
        }

        return retval.value();
    }

    fprintf(stderr, "%serror: invalid ast%s\n", COLOR_RED, RESET_TERM_COLOR);
    exit(EXIT_FAILURE);
}

void Matek::printast(void) const {
    if (!m_ast)
        return;

    m_ast->print();
    fprintf(stdout , "\n");
}

real_t Matek::evaluate() {
    Parser parser(m_expression);
    m_ast = parser.parse();

    return privateEval(m_ast);
}

void Matek::disableChecks(void) {
    docheck = false;
}
