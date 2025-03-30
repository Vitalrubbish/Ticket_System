#ifndef TOOLS_H
#define TOOLS_H
#include <string>
inline int stringToInt (const std::string& str) {
    int ans = 0, sign = 1;
    std::size_t pos = 0;
    if (str[pos] == '-') {
        ++pos;
        sign = -1;
    }
    while (pos < str.size()) {
        ans = ans * 10 + str[pos++] - '0';
    }
    return ans * sign;
}
#endif //TOOLS_H
