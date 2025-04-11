#pragma once

#include <iostream>
#include <iomanip>
#include <memory>

#include "real.h"
constexpr size_t DEFAULT_PRECISION = 3;

namespace Matek {
    struct BaseAst {
        virtual ~BaseAst() = default;
        virtual void print(size_t precision) const = 0;
    };

    /* NOTE: Here We are using std::string because we need a copy of operator,
     * and can't have a const char* since the stringStorage is always being filled with 0's */

    struct BinaryNode : public BaseAst {
        const std::string Operator;
        const std::unique_ptr<BaseAst> leftNode;
        const std::unique_ptr<BaseAst> rightNode;

        BinaryNode(const std::string& Operatorr, std::unique_ptr<BaseAst> leftnode, std::unique_ptr<BaseAst> rightnode) :
            Operator(Operatorr), leftNode(std::move(leftnode)), rightNode(std::move(rightnode)) {}

        void print(size_t precision) const override {
            std::cout << '(';
            leftNode->print(precision);

            std::cout << ' ' << Operator << ' ';
            rightNode->print(precision);

            std::cout <<  ") "; // for looks
        }
    };

    struct NumberNode : public BaseAst {
        const real_t value;
        NumberNode(const real_t _value) : value(_value) {}

        void print(size_t precision) const override {
            std::cout << std::fixed << std::setprecision(precision) <<  value;
        }
    };
};
