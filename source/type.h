#pragma once

#include "error.h"

namespace Matek {
    using __real__t = long double;

    struct ErrorData {
        char* readingSymbol;
        size_t currentPosition;
    };

    struct real_t {
        __real__t value;
        const Errorkind errorKind;
        const ErrorData errorData;

        real_t() : value(0), errorKind(None), errorData({0}) {}
    };
}

