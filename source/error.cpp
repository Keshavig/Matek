#include "error.h"

/* Todo: Replace that 1: with the nth line we are reading */
/* Todo: dont use std::string(....).c_str() rather make your own */

// wtf
namespace Error {
    void printer(const char* expression, const char* message, size_t spos, size_t endpos, char ch) {
        fprintf(stderr, "%s1:%zu%s: %serror: %s%s%s\n", COLOR_GREEN, spos, RESET_TERM_COLOR, COLOR_RED, COLOR_BLUE, message, RESET_TERM_COLOR);
        fprintf(stderr, "%s\n", expression);

        /* Note: This %*s stuff prints a space when the '*' = spos = 0 that's why the conditional is required */
        if (spos > 0) {
            fprintf(stderr, "%*s", (int) spos, " ");
        }

        fprintf(stderr, "%s%s%s\n", COLOR_YELLOW, std::string(endpos, ch).c_str(), RESET_TERM_COLOR);
    }
}


