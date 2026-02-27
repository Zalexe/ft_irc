#include "Utils.hpp"

/**
 * Compare a string against a pattern with '*' and '?' wildcards.
 * '*' matches 0 or more characters
 * '?' matches exactly 1 character
 */
bool wildcard_match(const char* str, const char* pattern) {
    const char* s = str;
    const char* p = pattern;
    const char* star = NULL;
    const char* ss = NULL;

    while (*s) {
        if (*p == '*') {
            star = p++;
            ss = s;
        }
        else if (*p == '?' || *p == *s) {
            ++p;
            ++s;
        }
        else if (star) {
            // backtrack to last '*'
            p = star + 1;
            s = ++ss;
        }
        else {
            return false;
        }
    }

    // Skip remaining '*' in pattern
    while (*p == '*') ++p;

    return *p == '\0';
}