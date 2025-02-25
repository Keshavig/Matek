#include <optional>
#include <iostream>

#include "parser.h"
#include "base_ast_node.h"

real_t eval(const std::unique_ptr<BaseAst>& ast) {
    if (auto numberNode = dynamic_cast<NumberNode*>(ast.get())) {
        return numberNode->value;
    }

    else if (auto binaryNode = dynamic_cast<BinaryNode*>(ast.get())) {
        real_t left = eval(std::move(binaryNode->leftNode));
        real_t right = eval(std::move(binaryNode->rightNode));

        std::optional<real_t> retval = operatorsList.runfunc(binaryNode->Operator, left, right);

        if (retval)
            return retval.value();

        fprintf(stderr, "%serror: invalid operator '%c' %s\n", COLOR_RED, binaryNode->Operator, RESET_TERM_COLOR);
        exit(1);
    }

    fprintf(stderr, "%serror: invalid ast%s\n", COLOR_RED, RESET_TERM_COLOR);
    exit(1);
}

void printast(const std::unique_ptr<BaseAst>& node) {
    if (!node)
        return;

    node->print();
    fprintf(stdout , "\n");
}

int main(void) {
    std::string expr;

    fprintf(stdout, "$ ");
    std::getline(std::cin, expr);

    Parser parse(expr);

    std::unique_ptr<BaseAst> parsed_output = parse.parse();
    printast(parsed_output);

    fprintf(stdout, "%.16Lf\n", eval(parsed_output));
    return 0;
}

