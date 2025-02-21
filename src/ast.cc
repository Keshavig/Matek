#include <cassert>
#include <cctype>
#include <cstdio>
#include <memory>
#include <string>
#include <utility>
#include <iostream>

#include "lexer.h"
#include "tokenType.h"

#define COLOR_RED "\033[38;2;255;108;107m" // Color #ff6c6b
#define RESET_TERM_COLOR "\033[0m"

/* TODO: Add 2*3*4*5 type support should be like (((2*3)*4)*5) [DONE]
 * TODO: Add parenthesis support
 * TODO: Extend for many things like variable search/matching function matching etc.
 * TODO: Add Evaluate function
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
        _operator(_o),
        leftNode(std::move(ln)),
        rightNode(std::move(rn)) {}

    char _operator;
    std::unique_ptr<BaseAst> leftNode;
    std::unique_ptr<BaseAst> rightNode;

    void print() const override {
        std::cout << "(";
        leftNode->print();

        std::cout << ' ' << _operator << ' ';
        rightNode->print();

        std::cout << ")";
    }
};

class NumberNode : public BaseAst {
public:
    double value;
    NumberNode(const double _value) : value(_value) {}

    void print() const override {
        std::cout << value;
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

        if (CurrentToken.tokenType == TokenType::OPERATOR && CurrentTokenPosition == 1) {
            std::unique_ptr<BaseAst> number = std::make_unique<NumberNode>(0.0);
            return number;
        }

        else if (CurrentToken.tokenType == TokenType::LPAREN) {
            std::unique_ptr<BaseAst> returnNode = parse_lp();
            return returnNode;
        }

        else if (CurrentToken.tokenType == TokenType::END) {
            fprintf(stderr, "%sinvalid call to parse_np; program failed; end of expression was reached%s\n",
                    COLOR_RED, RESET_TERM_COLOR);
            exit(1);
        }

        else if (CurrentToken.tokenType == TokenType::RPAREN) {
            fprintf(stderr, "%sfatal error: invalid syntax at %zu '%s'%s\n", COLOR_RED, CurrentTokenPosition,
                    CurrentToken.tokenSymbol.c_str(), RESET_TERM_COLOR);
        }

        else if (CurrentToken.tokenType != TokenType::NUMBER) {
            fprintf(stderr, "%sError invalid syntax %zu: '%s' %s\n", COLOR_RED, CurrentTokenPosition,
                    CurrentToken.tokenSymbol.c_str(), RESET_TERM_COLOR);

            exit(1);
        }

        std::string number;
        number = CurrentToken.tokenSymbol;
        std::cout << "Number: " << number << std::endl;
        std::unique_ptr<BaseAst> numberNode = std::make_unique<NumberNode>(std::stod(number));

        return numberNode;
    }

    std::unique_ptr<BaseAst> gethpNodes(std::unique_ptr<BaseAst> node) {

        /* If node = nullptr then parse_numbers else just copy the node into left node */
        std::unique_ptr<BaseAst> ln = !node ? parse_np() : std::move(node);
        getNewCurrentToken();
        char _operator = CurrentOperator;

        if (_operator == '+' || _operator == '-' || CurrentToken.tokenType == TokenType::END || CurrentToken.tokenType == TokenType::RPAREN)
        {
            return ln;
        }

        std::unique_ptr<BaseAst> rn = parse_np();
        std::unique_ptr<BaseAst> hpnode = std::make_unique<BinaryNode>(_operator,
                                                                       std::move(ln),
                                                                       std::move(rn));

        // getNewCurrentToken();
        return hpnode;
    }

    std::unique_ptr<BaseAst> parse_hp() {
        std::unique_ptr<BaseAst> hpnode = nullptr;

        /* This whole loop is for things like 2*3*4*... = ((2*3)*4)... */
        do {
            hpnode = gethpNodes(std::move(hpnode));
        } while (CurrentOperator != '+' && CurrentOperator != '-' && CurrentToken.tokenType != TokenType::END && CurrentToken.tokenType != TokenType::RPAREN && CurrentToken.tokenType != TokenType::END);

        return hpnode;
    }

    std::unique_ptr<BaseAst> getsubNodes(std::unique_ptr<BaseAst> node) {
        std::unique_ptr<BaseAst> leftnode = !node ? parse_hp() : std::move(node);

        /* For cases like: 2 or 3 when there is just one operand and nothing, we just return that operand*/
        if (CurrentToken.tokenType == TokenType::END || CurrentToken.tokenType == TokenType::RPAREN) 
            return leftnode;

        char _operator = CurrentOperator;
        std::unique_ptr<BaseAst> rightnode = parse_hp();

        std::unique_ptr<BaseAst> subnode = std::make_unique<BinaryNode>(_operator, std::move(leftnode), std::move(rightnode));
        return subnode;
    }

    std::unique_ptr<BaseAst> parse_lp() {
        std::unique_ptr<BaseAst> mainNode = nullptr;

        do {
            mainNode = getsubNodes(std::move(mainNode));
        } while (CurrentToken.tokenType != TokenType::END && CurrentToken.tokenType != TokenType::RPAREN);

        return mainNode;
    }
};

void printast(const std::unique_ptr<BaseAst>& ptr) {
    if (ptr) {
        ptr->print();  // Delegate the print operation to the appropriate node type's print function
        std::cout << std::endl;
    }
}

int main(void) {
    std::string expr;

    std::cout << "Enter a expression: ";
    std::getline(std::cin, expr);

    Parse parse(expr);
    std::cout << "Expression: " << expr << std::endl;

    std::unique_ptr<BaseAst> parsed_output = parse.startParsing();
    printast(std::move(parsed_output));
}

