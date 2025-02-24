# pragma once

#include <memory>

/* This function parses the tokens into an ast */

class Parser {
public:
    Parser() = delete;
    Parser(const Parser& p) = delete;

private:
    class BaseAst {
    public:
        virtual ~BaseAst() = default;
        virtual void print() const = 0;
    };

    std::unique_ptr<BaseAst> Parse();

};
