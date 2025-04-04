#pragma once

#include <cstdio>
#include <string>

constexpr const char* COLOR_RED    = "\033[31m";
constexpr const char* COLOR_BLUE   = "\033[34m";
constexpr const char* COLOR_PINK   = "\033[35m";
constexpr const char* COLOR_GREEN  = "\033[32m";
constexpr const char* COLOR_YELLOW = "\033[33m";
constexpr const char* BOLD_START   = "\033[1m";
constexpr const char* BOLD_END     = "\033[0m";

constexpr const char* RESET_TERM_COLOR = "\033[0m";

namespace Error {
    /* A function that prints error messages, PrintError */
    void printer(const char* expression, const char* message, size_t spos, size_t endpos = 1, char ch = '^');
}
