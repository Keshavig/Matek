#include "lexer.h"
#include "operators.h"

constexpr int DefaultIndexValue = -1;

/* TODO: Let the User Handle Errors */
/* Somethinig like SDL_GetError() would be nice with return values */

// Defined at @parser.cpp
extern void printer(const char* expression, const size_t position, const char* message, const bool color = true, const char ch = '^');

// NOTE: Can we something to reduce the allocation for stringStorage incase of really long expression?
// TODO: yes we can, We need to make a limit of 256 I think
namespace Matek 
{
    size_t strlen(const char* str) {
        size_t len = 0;
        while (str[len++]);

        return len;
    }

    static void clear(char* src) {
        const size_t srclen = strlen(src);
        for (size_t i = 0; i < srclen; ++i) src[i] = 0;
    }

    /* Constructor */
    Lexer::Lexer(const BinaryOperators& Operators, const std::string_view expression) :
        m_expression(expression), m_Operators(Operators), m_index(DefaultIndexValue), m_exprlen(expression.size()),
        stringStorage(new char[m_exprlen]) {}

    /* Destructor */
    Lexer::~Lexer() {
        delete[] stringStorage;
    }

    /* Note: https://www.ascii-code.com/ */
    static inline bool isvalidSpecialCharacter(const char ch) {
        return ( (ch >= '!' && ch <= '&' ) || (ch >= '*' && ch <= '-') || ch == '@' || ch == '^' || ch == '~' || ch == '/' );
    }

    static inline bool isletter(const char ch) {
        return (ch >= 65 && ch <= 95 ) || (ch >= 97 && ch <= 122);
    }

    static inline bool isaspace(const char ch) {
        return (ch == ' ' || ch == '\t' ||
                ch == '\n'|| ch == '\r' ||
                ch == '\v');
    }

    bool isadigit(const char ch) {
        return (ch == '1' || ch == '2' ||
                ch == '3' || ch == '4' ||
                ch == '5' || ch == '6' ||
                ch == '7' || ch == '8' ||
                ch == '9' || ch == '0' );
    }

    Token Lexer::getnextToken(void) {
        clear(stringStorage);
        ++m_index;

        if (m_index >= m_exprlen) {
            return {{}, TokenType::END}; // {} = null I think
        }

        /* Skip Spaces */
        while (isaspace(m_expression[m_index]))
            ++m_index;

        /* Collecting the number */
        size_t i = 0;
        for (i = 0; m_index < m_exprlen && (isadigit(m_expression[m_index]) || m_expression[m_index] == '.'); ++i) {
            stringStorage[i] = m_expression[m_index];
            char next = m_expression[m_index+1];
            if (isadigit(next) || next == '.') { 
                ++m_index;
                continue; 
            }

            return {stringStorage, TokenType::NUMBER};
        }

        if (m_expression[m_index] == '(' || m_expression[m_index] == ')') {
            stringStorage[0] = m_expression[m_index];
            stringStorage[1] = '\0';

            return {stringStorage, m_expression[m_index] == '(' ? TokenType::LPAREN : TokenType::RPAREN };
        }

        else if (isletter(m_expression[m_index]) || isvalidSpecialCharacter(m_expression[m_index])) {
            stringStorage[0] = m_expression[m_index];
            while (isletter(m_expression[m_index+1]) || isvalidSpecialCharacter(m_expression[m_index+1])) {
                stringStorage[++i] = m_expression[++m_index];
            }

            // TODO: Add varible + Function checking 
            for (size_t j = 0; j < m_Operators.length(); ++j) {
                singleOperator x = m_Operators.get(j);
                if (x.isvalidSymbol(stringStorage)) return {stringStorage, TokenType::OPERATOR };
            }
        }

        printer(m_expression.data(), m_index-i, "Unexpected token was found");
        exit(EXIT_FAILURE);
    }

    size_t Lexer::getindex(void) const {
        return m_index;
    }

    void Lexer::updateExpression(const char* expr) {
        m_expression = expr;
        m_index = DefaultIndexValue;
        m_exprlen = m_expression.size();
    }
}
