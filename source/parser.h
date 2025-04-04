#pragma once

#include "node.h"
#include "lexer.h"
#include "operators.h"

namespace Matek {
    class Parser {
        public:
            Parser(const Parser& anything) = delete;
            Parser() = delete;

            Parser(const BinaryOperators& Operators, const std::string_view expression);
            std::unique_ptr<BaseAst> parse();
            void updateExpression(const char* expre);

        private:
            std::string_view m_expression;
            Lexer ourlexer;

            /*  TODO: Organize these all into a struct or something */

            const BinaryOperators& m_Operators;

            /* NOTE: This should also store the operator's function idk */
            struct Data {
                char* operatorSymbol;
                char* tokenSymbol;
                Precedence operatorPrecedence;
                TokenType tokenType;
                size_t tokenPosition;
            } m_current;

        private:
            void updateCurrentToken(void);

            std::unique_ptr<BaseAst> parselowPrecedence();
            std::unique_ptr<BaseAst> getlowPrecedenceNodes(std::unique_ptr<BaseAst> node);

            std::unique_ptr<BaseAst> gethighPrecedenceNodes(std::unique_ptr<BaseAst> node);
            std::unique_ptr<BaseAst> parsehighPrecedence();

            std::unique_ptr<BaseAst> parseNumber();
    };
};
