#include "lexer.h"
#include "operators.h"

/*  TODO: Try to change most of std::string to either string_view or char* 
 *  To reduce allocation ==> Optimization :) */

/* TODO: Let users provide their own operators like: Matek start(Operators, Expression); */

// BinaryOperators Operators = {
//     { [](real_t a, real_t b) { return a+b; }, OperatorPrecedence::Low,  "+", "add" },
//     { [](real_t a, real_t b) { return a-b; }, OperatorPrecedence::Low,  "-", "sub" },
//     { [](real_t a, real_t b) { return a*b; }, OperatorPrecedence::High, "*", "mul" },
//     { [](real_t a, real_t b) { return a/b; }, OperatorPrecedence::High, "/", "div" },
// };

Lexer::Lexer(const BinaryOperators& Operators, const std::string& expression) :
    m_expression(expression),
    m_Operators(Operators),
    m_index(-1),
    m_operatorsLength(Operators.length()) /* NOTE: operatorsLength is only used at one place */
{
    if (m_expression.empty()) {
        std::cerr << "Empty expression\n";
        exit(EXIT_FAILURE);
    }
}

/*  NOTE: Check out to learn more about ascii stuff: https://www.ascii-code.com/ */
static bool isvalidSpecialCharacter(const char ch) {
    return ( (ch >= '!' && ch <= '&' ) || (ch >= '*' && ch <= '-') || ch == '@' || ch == '^' || ch == '~' || ch == '/' );
}

static bool isletter(const char ch) {
    return (ch >= 65 && ch <= 95 ) || (ch >= 97 && ch <= 122);
}

static bool isaspace(const char ch) {
    return (ch == ' ' || ch == '\t' ||
    ch == '\n'|| ch == '\r' ||
    ch == '\v');
}

static bool isadigit(const char ch) {
    return (ch == '1' || ch == '2' ||
    ch == '3' || ch == '4' ||
    ch == '5' || ch == '6' ||
    ch == '7' || ch == '8' ||
    ch == '9' || ch == '0' );
}

/* Just collects and returns a string */
std::string Lexer::collectSymbol(void) {
    std::string symbol;
    bool quit = false;
    char next = m_expression[m_index];

    do
    {
        symbol += next;
        next = m_expression[m_index+1];
        isletter(next) || isvalidSpecialCharacter(next) ? ++m_index : quit = true;

    }  while (m_index < m_expression.length() && !quit);

    return symbol;
}

Token Lexer::getnextToken(void) {
    ++m_index;

    const size_t exprlen = m_expression.length();
    if (m_index >= exprlen) {
        return {"", TokenType::END};
    }

    /* Skip Spaces */
    while (isaspace(m_expression[m_index])) ++m_index;

    std::string number;
    while (m_index < exprlen && isadigit(m_expression[m_index]) || m_expression[m_index] == '.')
    {
        number += m_expression[m_index];
        char next = m_expression[m_index+1];

        if (isadigit(next) || next == '.') {
            ++m_index;
            continue;
        }

        return {number, TokenType::NUMBER};
    }

    if (m_expression[m_index] == '(' || m_expression[m_index] == ')')
        return { std::string(1, m_expression[m_index]), m_expression[m_index] == '(' ? TokenType::LPAREN : TokenType::RPAREN };

    else if (isletter(m_expression[m_index]) || isvalidSpecialCharacter(m_expression[m_index])) {
        std::string symbol = collectSymbol();

        /* ------------------------------------------------------------------------ */
        /* TODO: check for variables, functions constant etc  */
        /* TODO: For Optimization ==> something with static hash_table */
        /* NOTE: Looping over OperatorsList checking if Readingstring is an operator */
        /* ------------------------------------------------------------------------- */

        for (size_t i = 0; i < m_operatorsLength; ++i) {
            singleOperator x = m_Operators.get(i);
            if (x.isvalidSymbol(symbol)) {
                return { symbol, TokenType::OPERATOR };
            }
        }
    }

    std::cerr << "ERROR: Invalid token at " << m_index << " `" << m_expression[m_index] << "`\n";
    exit(EXIT_FAILURE);
}

size_t Lexer::getindex(void) const {
    return m_index;
}
