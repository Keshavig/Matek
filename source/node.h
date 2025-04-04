#pragma once

#include <iostream>
#include <iomanip>
#include <memory>

constexpr size_t DEFAULT_PRECISION = 3;

using real_t = long double; /* also declared in @operators.h */

namespace Matek {
    class BaseAst {
        public:
            virtual ~BaseAst() = default;
            virtual void print(size_t precision) const = 0;

    };

    /* NOTE: Here We are using std::string because we need a copy of operator,
     * since the stringStorage pointer is always being cleared */

    class BinaryNode : public BaseAst {
        public:
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

    class NumberNode : public BaseAst {
        public:
            const real_t value;
            NumberNode(const real_t _value) : value(_value) {}

            void print(size_t precision) const override {
                std::cout << std::fixed << std::setprecision(precision) <<  value;
            }
    };
};
