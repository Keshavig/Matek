#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <cstdio>

#include "parser.h"
#include "operators.h"
#include "token.h"
#include "error.h"

constexpr const char* NULL_STRING = "";

namespace Matek {
    Parser::Parser(const BinaryOperators& Operators, const std::string_view expression) :
        m_expression(expression), ourlexer(Operators, expression), m_Operators(Operators) {}

    void Parser::updateCurrentToken(void) {
        Token t = ourlexer.getnextToken();

        m_current.tokenType = t.tokenType;
        m_current.tokenSymbol = const_cast<char*>(t.tokenSymbol);
        m_current.operatorSymbol = const_cast<char*>(m_current.tokenType == TokenType::OPERATOR ? m_current.tokenSymbol : NULL_STRING);
        m_current.operatorPrecedence = m_current.operatorSymbol == NULL_STRING ? Precedence::Invalid : m_Operators.getPrecedence(m_current.operatorSymbol);
        m_current.tokenPosition = ourlexer.getindex();
    }
    
    std::unique_ptr<BaseAst> Parser::handleNumber(void) {
        std::unique_ptr<BaseAst> numberNode;
        try {  
            numberNode = std::make_unique<NumberNode>(std::stold(m_current.tokenSymbol));
        }

        catch (std::invalid_argument& e) {
            Error::printer(m_current.tokenSymbol, "Invalid number", m_current.tokenPosition);
            exit(EXIT_FAILURE);
        }

        catch (std::out_of_range& e) {
            Error::printer(m_current.tokenSymbol, "Number out of bounds", m_current.tokenPosition);
            exit(EXIT_FAILURE);
        }

        updateCurrentToken();
        return numberNode;
    }

    std::unique_ptr<BaseAst> Parser::parseNumber(void) {
        updateCurrentToken();
        if (!m_current.tokenSymbol || m_current.tokenType == TokenType::END) {
            Error::printer(m_expression.data(), "Sudden end of expression, more input was required", m_current.tokenPosition);
            exit(EXIT_FAILURE);
        }

        switch(m_current.tokenType) {
            case TokenType::NUMBER:
                return handleNumber();

            case TokenType::LPAREN:
                return parselowPrecedence();

            default: break;
        }

        /* NOTE: for expression like +2, -3.141529 => (0+2), (0-3.141529) */
        if (m_current.tokenType == TokenType::OPERATOR &&  m_current.operatorPrecedence == Precedence::Low &&
                m_current.tokenPosition != m_expression.size()) {

            std::unique_ptr<BaseAst> lNumber = std::make_unique<NumberNode>(0);
            const char* Operator = m_current.operatorSymbol;
            std::unique_ptr<BaseAst> rNumber = parseNumber();

            return std::make_unique<BinaryNode>(Operator, std::move(lNumber), std::move(rNumber));
        }


        Error::printer(m_expression.data(), "Unexpected character was found", m_current.tokenPosition);
        exit(EXIT_FAILURE);
    }

    std::unique_ptr<BaseAst> Parser::gethighPrecedenceNodes(std::unique_ptr<BaseAst> node) {
        std::unique_ptr<BaseAst> ln = !node ? parseNumber() : std::move(node);
        std::unique_ptr<BaseAst> rn = nullptr;

        /* NOTE: Here we should do updateCurrentToken() right?*/
        /* NOTE: Nope dumb bitch */

        std::string Operator = m_current.operatorSymbol;

        if (m_current.operatorPrecedence == Precedence::Low || m_current.tokenType == TokenType::END ||
                m_current.tokenType == TokenType::RPAREN)  return ln;

        /* NOTE: This enables = 2(3) = 2*(3) instead of giving an error */
        else if (m_current.tokenType == TokenType::LPAREN) Operator = "*";

        rn = m_current.tokenType == TokenType::LPAREN ? parselowPrecedence() : parseNumber();
        return std::make_unique<BinaryNode>(Operator, std::move(ln), std::move(rn));
    }

    std::unique_ptr<BaseAst> Parser::parsehighPrecedence() {
        std::unique_ptr<BaseAst> hpnode = nullptr;

        /* NOTE: This loop is for expressions like 2*3*4*... = ((2*3)*4)..., each turn shifting the main `* root` */

        do { hpnode = gethighPrecedenceNodes(std::move(hpnode)); }
        while (m_current.operatorPrecedence != Precedence::Low && m_current.tokenType != TokenType::END && m_current.tokenType != TokenType::RPAREN);

        return hpnode;
    }

    std::unique_ptr<BaseAst> Parser::getlowPrecedenceNodes(std::unique_ptr<BaseAst> node) {
        std::unique_ptr<BaseAst> leftnode = !node ? parsehighPrecedence() : std::move(node); 

        /* For cases like: 2 or 3 when there is just one operand and nothing, we just return that operand*/
        if (m_current.tokenType == TokenType::END || m_current.tokenType == TokenType::RPAREN) { return leftnode; }

        const std::string Operator = m_current.operatorSymbol;
        std::unique_ptr<BaseAst> rightnode = parsehighPrecedence();

        std::unique_ptr<BaseAst> subnode = std::make_unique<BinaryNode>(Operator, std::move(leftnode), std::move(rightnode));
        return subnode;
    }

    std::unique_ptr<BaseAst> Parser::parselowPrecedence() {
        std::unique_ptr<BaseAst> mainNode = nullptr;

        do { mainNode = getlowPrecedenceNodes(std::move(mainNode)); }
        while (m_current.tokenType != TokenType::END && m_current.tokenType != TokenType::RPAREN);

        if (m_current.tokenType != TokenType::END) {
            updateCurrentToken(); /* This is for when currentToken = ')' in case like 2*(1+2) - 5*(1-0) */
        }

        return mainNode;
    }

    std::unique_ptr<BaseAst> Parser::parse() {
        return parselowPrecedence();
    }

    /* HACK: Could we somehow get rid this extra function in matek.h/cpp */
    void Parser::updateExpression(const char* expre) {
        m_expression = expre;
        ourlexer.updateExpression(expre);
    }
};
