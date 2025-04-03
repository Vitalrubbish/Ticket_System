#ifndef TOKENSCANNER_H
#define TOKENSCANNER_H
#include <string>

enum class TokenType { _c, _u, _p, _n, _m, _g, _i, _s, _x, _t, _o, _d, _y, _f, _q};
struct Tokens {
    int length = 0;
    TokenType tokenType[20]{};
    std::string op_name;
    std::string _c, _u, _p, _n, _m, _g, _i, _s, _x, _t, _o, _d, _y, _f, _q;
    std::string timeSlot;

    Tokens() = default;

    explicit Tokens(const std::string &str);

    ~Tokens() = default;

};
#endif //TOKENSCANNER_H
