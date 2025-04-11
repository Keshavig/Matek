#pragma once
#include <cstdio>

#include <string>

// Todo: Remove ERROR_ out of these names
enum class Errorkind {
    ERROR_SUDDEN_END_OF_EXPRESSION, /* Like `2 +` */
    ERROR_INVALID_PAREN_SYNTAX,
    ERROR_NUMBER_OUT_OF_RANGE, /* I have never exprienced this one */
    ERROR_EMPTY_EXPRESSION,
    ERROR_DIVISION_BY_ZERO,
    ERROR_INVALID_OPERATOR, /* Most of the time you will get this error: 2a, 2 ++ 3, 1asdkfkja all return this error*/
    ERROR_EXPECTED_NUMBER,
    ERROR_UNEQUAL_PARENS,
    ERROR_INVALID_SYNTAX,
    ERROR_I_DONT_KNOW, /* This just means the lib code is wrong causing ast to be weird and all*/
    ERROR_NONE,
};
