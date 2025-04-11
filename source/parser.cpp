#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <cstdio>

#include "parser.h"
#include "operators.h"
#include "token.h"
#include "error.h"

/* TODO: MOVE THROUGH ERRORS */
constexpr const char* NULL_STRING = "";

namespace Matek {
    Parser::Parser(const BinaryOperators& Operators, const std::string_view expression, Errorkind* errorkind) :
        m_expression(expression), ourlexer(Operators, expression, errorkind), m_Operators(Operators) {
            m_current.error = errorkind;
        }

    void Parser::updateCurrentToken(void) {
        Token t = ourlexer.getnextToken();

        m_current.tokenType = t.tokenType;
        m_current.tokenSymbol = const_cast<char*>(t.tokenSymbol);
        m_current.operatorSymbol = const_cast<char*>(m_current.tokenType == TokenType::OPERATOR ? m_current.tokenSymbol : NULL_STRING);
        m_current.operatorPrecedence = m_current.operatorSymbol == NULL_STRING ? Precedence::INVALID: m_Operators.getPrecedence(m_current.operatorSymbol);
        m_current.tokenPosition = ourlexer.getindex();
    }

    std::unique_ptr<BaseAst> Parser::handleNumber(void) {
        std::unique_ptr<BaseAst> numberNode;
        try { 
            numberNode = std::make_unique<NumberNode>(std::stold(m_current.tokenSymbol));
        }

        catch (std::out_of_range& e) {
            throw std::runtime_error("Number out of bounds");
            *m_current.error = Errorkind::ERROR_NUMBER_OUT_OF_RANGE;
            return nullptr;
        }

        updateCurrentToken();
        return numberNode;
    }

    std::unique_ptr<BaseAst> Parser::parseNumber(void) {
        updateCurrentToken();
        if (!m_current.tokenSymbol || m_current.tokenType == TokenType::END) {
            throw std::runtime_error("Sudden end of expression");
            *m_current.error = Errorkind::ERROR_SUDDEN_END_OF_EXPRESSION;
            return nullptr;
        }

        switch(m_current.tokenType) {
            case TokenType::NUMBER:
                return handleNumber();

            case TokenType::LPAREN:
                return parselowPrecedence();

            default: break;
        }

        if (m_current.tokenType == TokenType::OPERATOR && m_current.operatorPrecedence == Precedence::LOW
                && m_current.tokenPosition != m_expression.size()-1) {

            std::unique_ptr<BaseAst> lNumber = std::make_unique<NumberNode>(0);
            const std::string Operator = m_current.operatorSymbol;
            std::unique_ptr<BaseAst> rNumber = parseNumber();

            return std::make_unique<BinaryNode>(Operator, std::move(lNumber), std::move(rNumber));
        }

        throw std::runtime_error("I don't know what happend, we should have never reached here without a bug in the code");
        *m_current.error = Errorkind::ERROR_I_DONT_KNOW;
        return nullptr;
    }

    std::unique_ptr<BaseAst> Parser::gethighPrecedenceNodes(std::unique_ptr<BaseAst> node) {
        std::unique_ptr<BaseAst> ln = !node ? parseNumber() : std::move(node);
        std::unique_ptr<BaseAst> rn = nullptr;

        std::string Operator = m_current.operatorSymbol;
        if (m_current.operatorPrecedence == Precedence::LOW || m_current.tokenType == TokenType::END || m_current.tokenType == TokenType::RPAREN)
            return ln;

        else if (m_current.tokenType == TokenType::LPAREN) Operator = "*"; // for A(B) = A*B Evaluation
        rn = m_current.tokenType == TokenType::LPAREN ? parselowPrecedence() : parseNumber();
        return std::make_unique<BinaryNode>(Operator, std::move(ln), std::move(rn));
    }

    std::unique_ptr<BaseAst> Parser::parsehighPrecedence() {
        std::unique_ptr<BaseAst> hpnode = nullptr;

        /* NOTE: This loop is for expressions like 2*3*4*... = ((2*3)*4)..., each turn shifting the main `* root` */

        do { hpnode = gethighPrecedenceNodes(std::move(hpnode)); }
        while (m_current.operatorPrecedence != Precedence::LOW && m_current.tokenType != TokenType::END && m_current.tokenType != TokenType::RPAREN);

        return hpnode;
    }

    std::unique_ptr<BaseAst> Parser::getlowPrecedenceNodes(std::unique_ptr<BaseAst> node) {
        std::unique_ptr<BaseAst> leftnode = node ? std::move(node) : parsehighPrecedence(); 

        /* For cases like: 2 or 3 when there is just one operand and nothing, we just return that operand*/
        if (m_current.tokenType == TokenType::END || m_current.tokenType == TokenType::RPAREN) { return leftnode; }

        const std::string Operator = m_current.operatorSymbol;
        std::unique_ptr<BaseAst> rightnode = parsehighPrecedence();

        std::unique_ptr<BaseAst> subnode = std::make_unique<BinaryNode>(Operator, std::move(leftnode), std::move(rightnode));
        return subnode;
    }

    std::unique_ptr<BaseAst> Parser::parselowPrecedence() {
        std::unique_ptr<BaseAst> mainNode = nullptr;

        /* TODO: Convert this into while loop just cuz do loop looks UGLY as shit */
        do {
            mainNode = getlowPrecedenceNodes(std::move(mainNode));
        }

        while (m_current.tokenType != TokenType::END && m_current.tokenType != TokenType::RPAREN);

        /* This is for when currentToken = ')' in case like 2*(1+2) - 5*(1-0) */
        if (m_current.tokenType != TokenType::END) { updateCurrentToken(); }
        return mainNode;
    }

    std::unique_ptr<BaseAst> Parser::parse() {
        std::unique_ptr<BaseAst> x = parselowPrecedence();
        return x;
    }

    /* HACK: Could we somehow get rid this extra function in matek.h/cpp */
    void Parser::updateExpression(const char* expre) {
        m_expression = expre;
        ourlexer.updateExpression(expre);
        ourlexer.gntcallc = 0;
    }
};
