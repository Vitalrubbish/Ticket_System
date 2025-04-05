#ifndef USER_H
#define USER_H
#include "../database/BPT.h"
#include "tools.h"
#include <cstring>
#include <string>
struct User {
    char username[21]{};
    char password[31]{};
    char name[17]{};
    char mailAddr[31]{};
    int privilege{};
    bool login = false; // 存储用户的登录状态

    User() = default;

    explicit User(const std::string& _u);

    User(const std::string& _u, const std::string& _p, const std::string& _n,
                  const std::string& _m, const std::string& _g);

    bool operator< (const User& other) const;

    bool operator> (const User& other) const;

    bool operator<= (const User& other) const;

    bool operator>= (const User& other) const;

    bool operator== (const User& other) const;

    bool operator!= (const User& other) const;
};
#endif //USER_H
