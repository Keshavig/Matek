#pragma once

#include "error.h"

constexpr size_t ERRCODE = -1;

namespace Matek {
    using real_t = long double;

    struct ErrorData {
        char* readingSymbol;
        size_t currentPosition;
    };

    struct Real {
        real_t value;
        Errorkind errorKind;
        ErrorData errorData;

        Real() : value(0), errorKind(Errorkind::ERROR_NONE), errorData({0}) {}
        Real(const real_t a, const Errorkind b, const ErrorData c) : value(a), errorKind(b), errorData(c) {}

        Real(const Real& anotherOne) = default;
    };
}

