#include "lexer.h"
#include "operators.h"
#include "error.h"
#include "token.h"

constexpr int DefaultIndexValue = -1;
constexpr size_t strlenLimit = 6969;

/* TODO: Let the User Handle Errors */
/* Somethinig like SDL_GetError() would be nice with return values */

// NOTE: Can we something to reduce the allocation for storage incase of really long expression?
// TODO: yes we can, We need to make a limit of 256 I think

namespace Matek {
    size_t strlen(const char* str) {
        size_t len = 0;
        while (str[len++] && len < strlenLimit);

        return len;
    }

    static void clear(char* src) {
        const size_t srclen = strlen(src);
        for (size_t i = 0; i < srclen; ++i) src[i] = 0;
    }

    Lexer::Lexer(const BinaryOperators& Operators, const std::string_view expression, Errorkind* error) :
        m_expression(expression), m_Operators(Operators), m_index(DefaultIndexValue), m_exprlen(expression.size()),
        storage(new char[m_exprlen]), m_error(error), gntcallc(0) {}

    Lexer::~Lexer() {
        delete[] storage;
    }

    /* Note: https://www.ascii-code.com/ */
    static inline bool isvalidSpecialCharacter(const char ch) {
        return ch >= '!' && ch <= '&'  || ch >= '*' && ch <= '-' ||
            ch == '@' || ch == '^' || ch == '~' || ch == '/';
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
        clear(storage);
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
            storage[i] = m_expression[m_index];
            char next = m_expression[m_index+1];
            if (isadigit(next) || next == '.') { 
                ++m_index;
                continue; 
            }

            return {storage, TokenType::NUMBER};
        }

        if (m_expression[m_index] == '(' || m_expression[m_index] == ')') {
            storage[0] = m_expression[m_index];
            return {storage, m_expression[m_index] == '(' ? TokenType::LPAREN : TokenType::RPAREN };
        }

        if (isletter(m_expression[m_index]) || isvalidSpecialCharacter(m_expression[m_index]) && m_expression[m_index] != ' ') {
            if (gntcallc % 2 == 1) {
                *m_error = Errorkind::ERROR_EXPECTED_NUMBER;
                throw std::runtime_error("Catch this shit");
                return {{}, TokenType::INVALID};
            }

            storage[0] = m_expression[m_index];
            while (isletter(m_expression[m_index+1]) || isvalidSpecialCharacter(m_expression[m_index+1]) && m_expression[m_index] != ' ') {
                storage[++i] = m_expression[++m_index];
            }

            // TODO: Add varible + Function checking 
            for (size_t j = 0; j < m_Operators.length(); ++j) {
                singleOperator x = m_Operators.get(j);
                if (x.isvalidSymbol(storage)) return {storage, TokenType::OPERATOR };
            }
        }

        throw std::runtime_error("Catch this shit");
        *m_error = Errorkind::ERROR_I_DONT_KNOW;
        return {};
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
