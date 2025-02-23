#include <cassert>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <iostream>

#include "lexer.h"
#include "operators.h"
#include "tokenType.h"

extern BasicOperators operatorsList; // declared/defined in @lexer.h

#define COLOR_RED "\033[38;2;255;108;107m" // Color #ff6c6b
#define RESET_TERM_COLOR "\033[0m"

/* TODO: Add 2*3*4*5 type support should be like (((2*3)*4)*5) [DONE]
 * TODO: Add parenthesis support [DONE]
 * TODO: Add Evaluate function
 * TODO: Extend for many things like variable search/matching function matching etc.
 * TODO: Make ast.hpp header file; rather make parser.hpp header file
 * TODO: Allow User to even fucking have their own error messages
 * */


class BaseAst {
public:
    virtual ~BaseAst() = default;
    virtual void print() const = 0;
};

class BinaryNode : public BaseAst {
public:
    BinaryNode(char _o, std::unique_ptr<BaseAst> ln, std::unique_ptr<BaseAst> rn) :
        Operator(_o),
        leftNode(std::move(ln)),
        rightNode(std::move(rn)) {}

    const char Operator;
    const std::unique_ptr<BaseAst> leftNode;
    const std::unique_ptr<BaseAst> rightNode;

    void print() const override {
        fprintf(stdout, "(");
        leftNode->print();

        fprintf(stdout, " %c ", Operator);
        rightNode->print();

        fprintf(stdout, ")");
    }
};

class NumberNode : public BaseAst {
public:
    real_t value;
    NumberNode(const real_t _value) : value(_value) {}

    void print() const override {
        fprintf(stdout, "%.36Lf", value);
    }
};

class Parse {
private:
    const std::string m_expression;
    Lexer lex = m_expression;

    Token CurrentToken;
    char CurrentOperator;

    size_t CurrentTokenPosition;

    Token advance() {
        Token ct = lex.getnextToken();
        CurrentTokenPosition = lex.getindex();

        return ct;
    }

    bool setCurrentOperator() {
        if (CurrentToken.tokenType == TokenType::OPERATOR) {
            CurrentOperator = CurrentToken.tokenSymbol[0];
            return true;
        }

        CurrentOperator = '\0';
        return false;
    }

    bool getNewCurrentToken() {
        CurrentToken = advance();
        if (CurrentToken.tokenType == TokenType::END)
            return false;

        setCurrentOperator();
        return true;
    }

public:
    Parse(const std::string& expression) : m_expression(expression) {}
    std::unique_ptr<BaseAst> startParsing() {
        return parse_lp();
    }

    std::unique_ptr<BaseAst> parse_np() {
        getNewCurrentToken();

        /* this is for expression's like: -2, +3.14
         * we will just evaluate such input as: 0 - 2 */

        /* TODO: (CHECK) for ++2 or --2 and stuff like that */
        if (CurrentToken.tokenType == TokenType::OPERATOR &&
            (CurrentOperator == '+' || CurrentOperator == '-') && CurrentTokenPosition != m_expression.size())
        {

            std::unique_ptr<BaseAst> lNumber = std::make_unique<NumberNode>(0);
            char Operator = CurrentOperator;
            std::unique_ptr<BaseAst> rNumber = parse_np();

            return std::make_unique<BinaryNode>(Operator, std::move(lNumber), std::move(rNumber));
        }

        else if (CurrentToken.tokenType == TokenType::LPAREN) {
            std::unique_ptr<BaseAst> returnNode = parse_lp();
            return returnNode;
        }

        else if (CurrentToken.tokenType == TokenType::OPERATOR) {
            fprintf(stderr, "%sfatal error: invalid syntax at %zu: '%s'%s\n", COLOR_RED,
                    CurrentTokenPosition, CurrentToken.tokenSymbol.c_str(), RESET_TERM_COLOR);
            exit(1);
        }

        else if (CurrentToken.tokenType == TokenType::END) {
            fprintf(stderr, "%sfatal error: end of expression was reached%s\n",
                    COLOR_RED, RESET_TERM_COLOR);
            exit(1);
        }

        else if (CurrentToken.tokenType == TokenType::RPAREN) {
            fprintf(stderr, "| %sError: invalid syntax\n%s", COLOR_RED, RESET_TERM_COLOR);
            fprintf(stderr, "| %serror was found with symbol '%s' at position %zu%s\n", COLOR_RED,
                    CurrentToken.tokenSymbol.c_str(),
                    CurrentTokenPosition,
                    RESET_TERM_COLOR);;

            exit(1);

        }

        else if (CurrentToken.tokenType != TokenType::NUMBER) {
            fprintf(stderr, "%sError invalid syntax %zu: '%s' %s\n", COLOR_RED, CurrentTokenPosition,
                    CurrentToken.tokenSymbol.c_str(), RESET_TERM_COLOR);

            exit(1);
        }


        std::string number;
        number = CurrentToken.tokenSymbol;
        std::unique_ptr<BaseAst> numberNode;

        try {
            numberNode = std::make_unique<NumberNode>(std::stold(number));
        }

        catch (const std::invalid_argument& e) {
            fprintf(stdout, "%serror: invalid syntax (found: numbers containing characters)%s\n", COLOR_RED, RESET_TERM_COLOR);
            exit(1);
        }

        catch (const std::out_of_range& err) {
            fprintf(stdout, "%serror: %s number is way too big%s\n", COLOR_RED, number.c_str(), RESET_TERM_COLOR);
            exit(1);
        }

        return numberNode;
    }

    std::unique_ptr<BaseAst> gethpNodes(std::unique_ptr<BaseAst> node) {
        // Problem := (1+2) * (2-1) / (2+3)

        /* If node = nullptr then parse_numbers else just copy the node into left node */
        std::unique_ptr<BaseAst> ln = !node ? parse_np() : std::move(node);
        getNewCurrentToken();
        char Operator = CurrentOperator;

        if (Operator == '+' || Operator == '-' ||
            CurrentToken.tokenType == TokenType::END ||
            CurrentToken.tokenType == TokenType::RPAREN)
        {
            return ln;
        }

        std::unique_ptr<BaseAst> rn = parse_np();
        return std::make_unique<BinaryNode>(Operator, std::move(ln), std::move(rn));

    }

    std::unique_ptr<BaseAst> parse_hp() {
        std::unique_ptr<BaseAst> hpnode = nullptr;

        /* This whole loop is for things like 2*3*4*... = ((2*3)*4)... */
        do {
            hpnode = gethpNodes(std::move(hpnode));
        } while (
        CurrentOperator != '+' &&
        CurrentOperator != '-' &&
        CurrentToken.tokenType != TokenType::END &&
        CurrentToken.tokenType != TokenType::RPAREN &&
        CurrentToken.tokenType != TokenType::END );

        return hpnode;
    }

    std::unique_ptr<BaseAst> getsubNodes(std::unique_ptr<BaseAst> node) {
        std::unique_ptr<BaseAst> leftnode = !node ? parse_hp() : std::move(node); 


        /* For cases like: 2 or 3 when there is just one operand and nothing, we just return that operand*/
        if (CurrentToken.tokenType == TokenType::END || CurrentToken.tokenType == TokenType::RPAREN) 
            return leftnode;

        char Operator = CurrentOperator;
        std::unique_ptr<BaseAst> rightnode = parse_hp();

        std::unique_ptr<BaseAst> subnode = std::make_unique<BinaryNode>(Operator, std::move(leftnode), std::move(rightnode));
        return subnode;
    }

    std::unique_ptr<BaseAst> parse_lp() {
        std::unique_ptr<BaseAst> mainNode = nullptr;

        do {
            mainNode = getsubNodes(std::move(mainNode));
        } while (CurrentToken.tokenType != TokenType::END && CurrentToken.tokenType != TokenType::RPAREN);

        getNewCurrentToken(); /* This is for when currentToken = ')' in case like 2*(1+2) - 5*(1-0) */
        return mainNode;
    }
};

void printast(const std::unique_ptr<BaseAst>& ptr) {
    if (!ptr)
        return;

    ptr->print();
    fprintf(stdout , "\n");
}

real_t eval(const std::unique_ptr<BaseAst>& ast) {
    if (auto numberNode = dynamic_cast<NumberNode*>(ast.get())) {
        return numberNode->value;
    }

    else if (auto binaryNode = dynamic_cast<BinaryNode*>(ast.get())) {
        real_t left = eval(std::move(binaryNode->leftNode));
        real_t right = eval(std::move(binaryNode->rightNode));

        std::optional<real_t> retval = operatorsList.runfun(binaryNode->Operator, left, right);

        if (!retval) {
            fprintf(stderr, "%serror: invalid operator '%c' %s\n", COLOR_RED, binaryNode->Operator, RESET_TERM_COLOR);
            exit(1);
        }
        return retval.value();

    }

    fprintf(stderr, "%serror: invalid ast%s\n", COLOR_RED, RESET_TERM_COLOR);
    exit(1);
}

int main(void) {
    std::string expr;

    fprintf(stdout, "=> ");
    std::getline(std::cin, expr);

    Parse parse(expr);

    std::unique_ptr<BaseAst> parsed_output = parse.startParsing();
    printast(parsed_output);

    fprintf(stdout, "%.16Lf\n", eval(parsed_output));
    return 0;
}

