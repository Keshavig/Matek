#include <cstdio>

#include "operators.h"
#include "tokenType.h"
#include "lexer.h"

real_t divide(real_t a, real_t b) {
    if (b != 0)
        return a/b;

    fprintf(stderr, "math error: division by zero was found\n");
    exit(1);
}

BasicOperators operatorsList = {
    { '+', [](real_t a, real_t b) -> real_t { return a+b; } },
    { '-', [](real_t a, real_t b) -> real_t { return a-b; } },
    { '*', [](real_t a, real_t b) -> real_t { return a*b; } },
    { '/', divide},
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
    if (m_expression.empty()) {
        fprintf(stderr, "expression is empty!!\n");
        exit(1);
    }

    const size_t exprlen = m_expression.length();

    if (currentTokenPosition >= exprlen) {
        return {"", TokenType::END};
    }

    /* Skip Spaces */
    while (isaspace(m_expression[currentTokenPosition])) {
        ++currentTokenPosition;
    }

    /* Storing the number */
    if ( isadigit(m_expression[currentTokenPosition]) || m_expression[currentTokenPosition] == '.') {
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

    for (size_t i = 0; i < operatorsList.size(); i++) {
        if (m_expression[currentTokenPosition] == operatorsList.get(i).operatorSymbol) {
            return { std::string(1, m_expression[currentTokenPosition++]), TokenType::OPERATOR }; // Return then increment
        }
    }

    fprintf(stderr, "found Unexpected token at %zu '%c'\n", currentTokenPosition, m_expression.c_str()[currentTokenPosition]);
    exit(1);
}

size_t Lexer::getindex(void) const {
    return currentTokenPosition;
}
