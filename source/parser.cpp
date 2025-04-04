#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <cstdio>

#include "parser.h"
#include "operators.h"
#include "token.h"

constexpr const char* NULL_STRING = "";

constexpr const char* COLOR_RED    = "\033[31m";
constexpr const char* COLOR_BLUE   = "\033[34m";
constexpr const char* COLOR_PINK   = "\033[35m";
constexpr const char* COLOR_GREEN  = "\033[32m";
constexpr const char* COLOR_YELLOW = "\033[33m";

constexpr const char* RESET_TERM_COLOR = "\033[0m";

/* TODO: Replace that 1: with current line of input reading */

/* A function that prints error messages, PrintError */
void printer(const char* expression, const size_t position, const char* message, bool color = true, const char ch = '^') {
    if (!color) {
        fprintf(stderr, "1:%zu: error: %s\n", position, message);
        fprintf(stderr, "%s\n", expression);
        fprintf(stderr, "%*c\n", static_cast<int>(position+1), ch);
        return;
    }

    fprintf(stderr, "%s1:%zu%s: %serror:%s %s\n", COLOR_GREEN, position,
            RESET_TERM_COLOR, COLOR_RED, RESET_TERM_COLOR, message);

    fprintf(stderr, "%s%s%s\n", COLOR_BLUE, expression, RESET_TERM_COLOR);
    fprintf(stderr, "%s%*c%s\n", COLOR_RED, static_cast<int>(position), ch, RESET_TERM_COLOR);
}

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

    std::unique_ptr<BaseAst> Parser::parseNumber(void) {
        updateCurrentToken();

        if (m_current.tokenType == TokenType::NUMBER)
        {
            std::unique_ptr<BaseAst> numberNode;
            try {  
                numberNode = std::make_unique<NumberNode>(std::stold(m_current.tokenSymbol));
            }

            catch (std::invalid_argument& e) {
                printer(m_current.tokenSymbol, m_current.tokenPosition, "Invalid number");
                exit(EXIT_FAILURE);
            }

            catch (std::out_of_range& e) {
                printer(m_current.tokenSymbol, m_current.tokenPosition, "Number out of bounds");
                exit(EXIT_FAILURE);
            }

            updateCurrentToken();
            return numberNode;
        }

        /* NOTE: for expression like +2, -3.141529 => (0+2), (0-3.141529) */
        else if (m_current.tokenType == TokenType::OPERATOR &&  m_current.operatorPrecedence == Precedence::Low &&
                m_current.tokenPosition != m_expression.size()) {

            std::unique_ptr<BaseAst> lNumber = std::make_unique<NumberNode>(0);
            const char* Operator = m_current.operatorSymbol;
            std::unique_ptr<BaseAst> rNumber = parseNumber();

            return std::make_unique<BinaryNode>(Operator, std::move(lNumber), std::move(rNumber));
        }

        else if (m_current.tokenType == TokenType::LPAREN) {
            return parselowPrecedence();
        }

        /* FIXME: this feels unnecessary */
        else if (m_current.tokenType == TokenType::END) {
            // We should return back to the user's code here
            static_assert(true);
        }

        printer(m_expression.data(), m_current.tokenPosition, "Unexpected character was found");
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
