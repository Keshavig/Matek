#pragma once

#include <string_view>

/* This will do stuff like checking if no of opening and ending parenthesis are equal and stuff */

class Checks {
private:
    const std::string_view m_expression;

public:
    bool checkParenCount(void) const;
    bool checkParenSyntax(void) const;
    bool checkOperators(void) const; // check for things like check if #(operators) = #(numbers) - 1 ifnot error
 
    Checks(const std::string_view& expression) : m_expression(expression) {};

    Checks(const Checks& chks) = delete;
    Checks() = delete;
};

