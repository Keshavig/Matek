#pragma once

/* This file parses the tokens into an ast */

#include <memory>
#include <string>

#include "node.h"
#include "lexer.h"

// declared/defined in @lexer.cc
extern BasicOperators Operators;

#define COLOR_RED "\033[38;2;255;108;107m" // Color #ff6c6b
#define RESET_TERM_COLOR "\033[0m"

class Parser {
public:
    Parser(const Parser &) = delete;
    Parser(const std::string& expression);

    std::unique_ptr<BaseAst> parse();

private:
    const std::string m_expression;
    Lexer lexer;

    std::string m_currentTokenSymbol;
    TokenType m_currentTokenType;
    char m_currentOperator;
    size_t m_currentTokenPosition;

private:
    void getNewCurrentToken(void);

    std::unique_ptr<BaseAst> parse_lowPrecedence();
    std::unique_ptr<BaseAst> get_lowPrecedenceNodes(std::unique_ptr<BaseAst> node);

    std::unique_ptr<BaseAst> parse_highPrecedence();
    std::unique_ptr<BaseAst> get_highPrecedenceNodes(std::unique_ptr<BaseAst> node);

    std::unique_ptr<BaseAst> parse_Number();
    std::unique_ptr<BaseAst> parse_Paren();
};
