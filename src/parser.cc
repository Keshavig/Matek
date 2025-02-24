#include <string>
#include <stdexcept>

#include "parser.h"
#include "token.h"
#include "operators.h"

Parser::Parser(const std::string& expression) : m_expression(expression), lexer(expression) {}

void Parser::getNewCurrentToken(void) {
    Token t = lexer.getnextToken();

    m_currentTokenType = t.tokenType;
    m_currentTokenSymbol = t.tokenSymbol;

    m_currentOperator = m_currentTokenType == TokenType::OPERATOR ? m_currentTokenSymbol[0] : 0;
    m_currentTokenPosition = lexer.getindex();
}

std::unique_ptr<BaseAst> Parser::parse_NumberAndParen(void) {
        getNewCurrentToken();

        /* this is for expression's like: -2, +3.14
         * we will just evaluate such input as: 0 - 2 */

        /* TODO: (CHECK) for ++2 or --2 and stuff like that
         * TODO: try to make this function more concise(spelling?) */
        if (m_currentTokenType == TokenType::OPERATOR &&
            (m_currentOperator == '+' || m_currentOperator == '-') && m_currentTokenPosition != m_expression.size())
        {

            std::unique_ptr<BaseAst> lNumber = std::make_unique<NumberNode>(0);
            char Operator = m_currentOperator;
            std::unique_ptr<BaseAst> rNumber = parse_NumberAndParen();

            return std::make_unique<BinaryNode>(Operator, std::move(lNumber), std::move(rNumber));
        }

        else if (m_currentTokenType == TokenType::LPAREN) {
            std::unique_ptr<BaseAst> returnNode = parse_lowPrecedence();
            return returnNode;
        }

        else if (m_currentTokenType == TokenType::OPERATOR) {
            fprintf(stderr, "%sfatal error: invalid syntax at %zu: '%s'%s\n", COLOR_RED,
                    m_currentTokenPosition, m_currentTokenSymbol.c_str(), RESET_TERM_COLOR);
            exit(1);
        }

        else if (m_currentTokenType == TokenType::END) {
            fprintf(stderr, "%sfatal error: end of expression was reached%s\n",
                    COLOR_RED, RESET_TERM_COLOR);
            exit(1);
        }

        else if (m_currentTokenType != TokenType::NUMBER) {
            fprintf(stderr, "%sError: invalid syntax\n%s", COLOR_RED, RESET_TERM_COLOR);
            fprintf(stderr, "%serror was found with symbol '%s' at position %zu%s\n", COLOR_RED,
                    m_currentTokenSymbol.c_str(),
                    m_currentTokenPosition,
                    RESET_TERM_COLOR);;

            exit(1);
        }

        std::string number;
        number = m_currentTokenSymbol;
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

        getNewCurrentToken();
        return numberNode;
    }


std::unique_ptr<BaseAst> Parser::get_highPrecedenceNodes(std::unique_ptr<BaseAst> node) {
    std::unique_ptr<BaseAst> ln = !node ? parse_NumberAndParen() : std::move(node);
    char Operator = m_currentOperator;

    if (Operator == '+' || Operator == '-' ||
        m_currentTokenType == TokenType::END ||
        m_currentTokenType == TokenType::RPAREN)
    {
        return ln;
    }

    std::unique_ptr<BaseAst> rn = parse_NumberAndParen();
    return std::make_unique<BinaryNode>(Operator, std::move(ln), std::move(rn));

}


std::unique_ptr<BaseAst> Parser::parse_highPrecedence() {
    std::unique_ptr<BaseAst> hpnode = nullptr;

    /* This whole loop is for things like 2*3*4*... = ((2*3)*4)... */
    do {
        hpnode = get_highPrecedenceNodes(std::move(hpnode));
    } while (
    m_currentOperator     != '+'
    && m_currentOperator  != '-'
    && m_currentTokenType != TokenType::END
    && m_currentTokenType != TokenType::RPAREN );

    return hpnode;
}

std::unique_ptr<BaseAst> Parser::get_lowPrecedenceNodes(std::unique_ptr<BaseAst> node) {
    std::unique_ptr<BaseAst> leftnode = !node ? parse_highPrecedence() : std::move(node); 


    /* For cases like: 2 or 3 when there is just one operand and nothing, we just return that operand*/
    if (m_currentTokenType == TokenType::END || m_currentTokenType == TokenType::RPAREN) 
        return leftnode;

    /* no checks on Operator?? in check.h right? */
    char Operator = m_currentOperator;
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

