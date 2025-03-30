#include "../include/User.h"

User::User(const std::string& _u) {
    strcpy(username, _u.c_str());
}

User::User(const std::string& _u, const std::string& _p, const std::string& _n,
              const std::string& _m, const std::string& _g) {
    strcpy(username, _u.c_str());
    strcpy(password, _p.c_str());
    strcpy(name, _n.c_str());
    strcpy(mailAddr, _m.c_str());
    privilege = stringToInt(_g);
}

bool User::operator< (const User& other) const {
    std::string str1(username, strlen(username));
    std::string str2(other.username, strlen(other.username));
    return str1 < str2;
}

bool User::operator> (const User& other) const {
    std::string str1(username, strlen(username));
    std::string str2(other.username, strlen(other.username));
    return str1 > str2;
}

bool User::operator<= (const User& other) const {
    std::string str1(username, strlen(username));
    std::string str2(other.username, strlen(other.username));
    return str1 <= str2;
}

bool User::operator>= (const User& other) const {
    std::string str1(username, strlen(username));
    std::string str2(other.username, strlen(other.username));
    return str1 >= str2;
}

bool User::operator== (const User& other) const {
    std::string str1(username, strlen(username));
    std::string str2(other.username, strlen(other.username));
    return str1 == str2;
}

bool User::operator!= (const User& other) const {
    std::string str1(username, strlen(username));
    std::string str2(other.username, strlen(other.username));
    return str1 != str2;
}