#pragma once

#include <memory>

#ifdef __DEBUG__
#include <cstdio>
#endif

using real_t = long double; /* also declared in @operators.h */

class BaseAst {
public:
    virtual ~BaseAst() = default;

#ifdef __DEBUG__
    virtual void print() const = 0;
#endif

};

class BinaryNode : public BaseAst {
public:
    BinaryNode(char Operatorr, std::unique_ptr<BaseAst> leftnode, std::unique_ptr<BaseAst> rightnode) :
        Operator(Operatorr),
        leftNode(std::move(leftnode)),
        rightNode(std::move(rightnode)) {}

    const char Operator;
    const std::unique_ptr<BaseAst> leftNode;
    const std::unique_ptr<BaseAst> rightNode;

#ifdef __DEBUG__
    void print() const override {
        fprintf(stdout, "(");
        leftNode->print();

        fprintf(stdout, " %c ", Operator);
        rightNode->print();

        fprintf(stdout, ")");
    }
#endif

};

class NumberNode : public BaseAst {
public:
    real_t value;
    NumberNode(const real_t _value) : value(_value) {}

#ifdef __DEBUG__
    void print() const override {
        fprintf(stdout, "%.5Lf", value);
    }
#endif

};

