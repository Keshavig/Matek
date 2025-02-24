#pragma once

#include <string>

/* This will do stuff like checking if no of opening and ending parenthesis are equal and stuff */

class checks {
private:
    const std::string m_expression;
public:
    bool checkParen(void) const; // check for things like (( 2 or (2-1))
    bool checkOperators(void) const; // check for things like 2 ++ 2
 
    checks(const std::string& expression) : m_expression() {};
    checks(const checks& chks) = delete;
};

