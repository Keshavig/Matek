#include <cstdio>
#include <string_view>

#include "lexer.h"

Lexer::Lexer(const std::string_view expression) : m_expression(expression.data()), currentTokenPosition(0) {
    if (m_expression.empty()) {
        fprintf(stderr, "Empty Expression\n");
        exit(1);
    }
}

real_t divide(real_t a, real_t b) {
    if (b != 0)
        return a/b;

    fprintf(stderr, "Math Error: Division by zero\n");
    exit(1);
}

BasicOperators Operators = {
    { '+', OperatorPrecedence::LOW , [](real_t a, real_t b) -> real_t { return a+b; } },
    { '-', OperatorPrecedence::LOW , [](real_t a, real_t b) -> real_t { return a-b; } },
    { '*', OperatorPrecedence::HIGH, [](real_t a, real_t b) -> real_t { return a*b; } },
    { '/', OperatorPrecedence::HIGH, divide},
};

bool Lexer::isaspace(const char ch) {
    return (ch == ' ' || ch == '\t' ||
    ch == '\n'|| ch == '\r' ||
    ch == '\v');
}

bool Lexer::isadigit(const char ch) {
    return (ch == '1' || ch == '2' ||
    ch == '3' || ch == '4' ||
    ch == '5' || ch == '6' ||
    ch == '7' || ch == '8' ||
    ch == '9' || ch == '0' );
}

Token Lexer::getnextToken(void) {
    const size_t exprlen = m_expression.length();
    if (currentTokenPosition >= exprlen) {
        return {"", TokenType::END};
    }

    /* Skip Spaces */
    while (isaspace(m_expression[currentTokenPosition])) {
        ++currentTokenPosition;
    }

    /* Storing the number */
    if (isadigit(m_expression[currentTokenPosition]) || m_expression[currentTokenPosition] == '.') {
        std::string number = "";
        char currentReadingToken = m_expression[currentTokenPosition];

        while ( ( isadigit(currentReadingToken) || currentReadingToken == '.') && (currentTokenPosition < exprlen) ) {
            number += currentReadingToken;
            currentReadingToken = m_expression[++currentTokenPosition];
        }

        return {number, TokenType::NUMBER};
    }

    else if (m_expression[currentTokenPosition] == '(') {
        ++currentTokenPosition;
        return { "(", TokenType::LPAREN };
    }

    else if (m_expression[currentTokenPosition] == ')' ) {
        ++currentTokenPosition;
        return { ")", TokenType::RPAREN };
    }

    for (size_t i = 0; i < Operators.size(); i++) {
        if (m_expression[currentTokenPosition] == Operators.get(i).operatorSymbol) {
            return { std::string(1, m_expression[currentTokenPosition++]), TokenType::OPERATOR }; // Return then increment
        }
    }

    fprintf(stderr, "Found Unexpected token at %zu '%c'\n", currentTokenPosition, m_expression[currentTokenPosition]);
    exit(1);
}

size_t Lexer::getindex(void) const {
    return currentTokenPosition;
}
