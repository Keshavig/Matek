#include "checks.h"
#include "operators.h"

/* This is defined in @lexer.cc */
extern BasicOperators basicOperators;

inline static bool isaspace(const char character) {
    return (character == ' '  || character == '\t' ||
    character == '\v' || character == '\n' ||
    character == '\a');
}

static size_t getOccurence(const std::string& tofindin, const char character) {
    size_t length = tofindin.length();
    size_t occurenceCount = 0;

    for (size_t i = 0; i < length; i++) {
        if (tofindin[i] == character)
            occurenceCount++;
    }

    return occurenceCount;
}

// for str = "abcc", and character = 'c' this shall return 2+3 = 5
static size_t getOccurencesPositionSum(const std::string& str, const char character) {
    size_t sum = 0, length = str.length();

    for (size_t i = 0; i < length; ++i) {
        if (str[i] == character)
            sum += i;
    }

    return sum;
}

bool Checks::checkParenCount(void) const {
    size_t leftParenOccurenceCount  = getOccurence(m_expression, '(');
    size_t rightParenOccurenceCount = getOccurence(m_expression, ')');

    if (leftParenOccurenceCount != rightParenOccurenceCount)
        return false;

    return true;
}

bool Checks::checkParenSyntax(void) const {
    size_t leftParenSum  = getOccurencesPositionSum(m_expression, '(');
    size_t rightParenSum = getOccurencesPositionSum(m_expression, ')');

    if (leftParenSum > rightParenSum)
        return false;

    return true;
}
