#ifndef TOOLS_H
#define TOOLS_H
#include <string>
#include "../STLite/vector.hpp"
inline int stringToInt(const std::string& str) {
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

inline sjtu::vector<std::string> split(const std::string& str,const char& c) {
    sjtu::vector<std::string> ret;
    std::size_t pre = 0;
    for (std::size_t i = 0; i < str.size(); i++) {
        if (str[i] == c || i == str.size() - 1) {
            ret.push_back(str.substr(pre, i - pre));
            pre = i + 1;
        }
    }
    return ret;
}
#endif //TOOLS_H