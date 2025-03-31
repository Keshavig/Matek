#include "parser.h"
#include "operators.h"

// These all are just to show what stdlibs are being used, this will work 
// even if we remove them (as they are already included in ^ header files)

// TOOD: make m_currentOperatorPrecedence

#include <cstdio>
#include <iostream>
#include <memory>

/* NOTE: Here we are not checking if `Operator` is a valid operator or not because that is already checked by the lexer */
static OperatorPrecedence getPrecedence(const std::string& Operator) {
    singleOperator sopr = Operators.get(Operator);
    return sopr.getPrecedence();
}

Parser::Parser(const std::string& expression) : m_expression(expression), lexer(expression) {}

void Parser::getNewCurrentToken(void) {
    Token t = lexer.getnextToken();

    m_currentTokenType = t.tokenType;
    m_currentTokenSymbol = t.tokenSymbol;

    m_currentOperator = m_currentTokenType == TokenType::OPERATOR ? m_currentTokenSymbol : "";
    m_currentTokenPosition = lexer.getindex();
}

std::unique_ptr<BaseAst> Parser::parse_Paren() {
    std::unique_ptr<BaseAst> node = parse_lowPrecedence();
    return node;
}

std::unique_ptr<BaseAst> Parser::parse_Number(void) {
    getNewCurrentToken();

    /* NOTE: this is for expression's like: -2, +3.14
         * we will just evaluate such input as: 0 - 2 */

    /* 
    if (m_currentTokenType == TokenType::OPERATOR &&
       (m_currentOperator == '+' || m_currentOperator == '-') && m_currentTokenPosition != m_expression.size()) {
    */

    if (m_currentTokenType == TokenType::OPERATOR &&  getPrecedence(m_currentOperator) == OperatorPrecedence::Low &&
        m_currentTokenPosition != m_expression.size()) {

        std::unique_ptr<BaseAst> lNumber = std::make_unique<NumberNode>(0);
        const std::string Operator = m_currentOperator;
        std::unique_ptr<BaseAst> rNumber = parse_Number();

        /*  TODO: BINARYNODE [ NEED TO CONVERT OPERATOR NAME TO SYMBOL ] */
        return std::make_unique<BinaryNode>(Operator, std::move(lNumber), std::move(rNumber));
    }

    else if (m_currentTokenType == TokenType::LPAREN) {
        return parse_Paren();
    }

    else if (m_currentTokenType == TokenType::OPERATOR) {
        std::cout << COLOR_RED << "fatal error: invalid syntax at " << m_currentTokenPosition << \
            ": " << m_currentTokenSymbol  << RESET_TERM_COLOR << '\n';
        exit(EXIT_FAILURE);
    }

    /* NOTE: this feels unnecessary */
    else if (m_currentTokenType == TokenType::END) {
        fprintf(stderr, "%sfatal error: end of expression was reached%s\n",
                COLOR_RED, RESET_TERM_COLOR);
        exit(EXIT_FAILURE);
    }

    /*  NOTE: this and TokenType::Operator check should be combined */
    else if (m_currentTokenType != TokenType::NUMBER) {
        fprintf(stderr, "%sError: invalid syntax\n%s", COLOR_RED, RESET_TERM_COLOR);
        std::cerr << COLOR_RED << "invalid character was found at position " << m_currentTokenPosition << \
            " of " << m_currentTokenSymbol << RESET_TERM_COLOR << '\n';

        exit(EXIT_FAILURE);
    }

    const std::string number(m_currentTokenSymbol);
    std::unique_ptr<BaseAst> numberNode;

    try {
        numberNode = std::make_unique<NumberNode>(std::stold(number));
    }

    catch (const std::invalid_argument& e) {
        std::cerr << COLOR_RED << "Error: invalid syntax " << number << '\n' << RESET_TERM_COLOR;
        exit(EXIT_FAILURE);
    }

    catch (const std::out_of_range& err) {
        std::cerr << COLOR_RED << "Error: number out of bounds\n" << RESET_TERM_COLOR;
        exit(EXIT_FAILURE);
    }

    getNewCurrentToken();
    return numberNode;
}


std::unique_ptr<BaseAst> Parser::get_highPrecedenceNodes(std::unique_ptr<BaseAst> node) {
    std::unique_ptr<BaseAst> ln = !node ? parse_Number() : std::move(node);
    std::unique_ptr<BaseAst> rn = nullptr;

    std::string Operator = m_currentOperator;
    bool unusual_token = getPrecedence(Operator) == OperatorPrecedence::Low || m_currentTokenType == TokenType::END || m_currentTokenType == TokenType::RPAREN;

    if (unusual_token) {
        return ln;
    }

    /* for like 2(3) = 2*(3) */
    /* Only do this if '*' [Multiplication] Operator Exists */

    else if (m_currentTokenType == TokenType::LPAREN) {
        Operator = "*";
    }

    rn = m_currentTokenType == TokenType::LPAREN ? parse_Paren() : parse_Number();
    return std::make_unique<BinaryNode>(Operator, std::move(ln), std::move(rn));

}


std::unique_ptr<BaseAst> Parser::parse_highPrecedence() {
    std::unique_ptr<BaseAst> hpnode = nullptr;

    /* This whole loop is for things like 2*3*4*... = ((2*3)*4)... */

    do
    { 
        hpnode = get_highPrecedenceNodes(std::move(hpnode));
    }
    while (getPrecedence(m_currentOperator) != OperatorPrecedence::Low &&
    m_currentTokenType != TokenType::END && m_currentTokenType != TokenType::RPAREN);

    return hpnode;
}

std::unique_ptr<BaseAst> Parser::get_lowPrecedenceNodes(std::unique_ptr<BaseAst> node) {
    std::unique_ptr<BaseAst> leftnode = !node ? parse_highPrecedence() : std::move(node); 


    /* For cases like: 2 or 3 when there is just one operand and nothing, we just return that operand*/
    if (m_currentTokenType == TokenType::END || m_currentTokenType == TokenType::RPAREN) 
        return leftnode;

    /* no checks on Operator?? in check.h right? */
    const std::string Operator = m_currentOperator;
    std::unique_ptr<BaseAst> rightnode = parse_highPrecedence();

    std::unique_ptr<BaseAst> subnode = std::make_unique<BinaryNode>(Operator, std::move(leftnode), std::move(rightnode));
    return subnode;
}

std::unique_ptr<BaseAst> Parser::parse_lowPrecedence() {

    std::unique_ptr<BaseAst> mainNode = nullptr;

    do {
        mainNode = get_lowPrecedenceNodes(std::move(mainNode));
    } while (m_currentTokenType != TokenType::END && m_currentTokenType != TokenType::RPAREN);

    getNewCurrentToken(); /* This is for when currentToken = ')' in case like 2*(1+2) - 5*(1-0) */
    return mainNode;
}

std::unique_ptr<BaseAst> Parser::parse() {
    return parse_lowPrecedence();
}

