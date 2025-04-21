#include "../include/User.h"

User::User(const std::string& _u) {
    hashCode = HashByByte(_u);
}

User::User(const std::string& _u, const std::string& _p, const std::string& _n,
              const std::string& _m, const std::string& _g) {
    strcpy(username, _u.c_str());
    strcpy(password, _p.c_str());
    strcpy(name, _n.c_str());
    strcpy(mailAddr, _m.c_str());
    privilege = stringToInt(_g);
    hashCode = HashByByte(_u);
}

bool User::operator< (const User& other) const {
    return hashCode < other.hashCode;
}

bool User::operator> (const User& other) const {
    return hashCode > other.hashCode;
}

bool User::operator<= (const User& other) const {
    return hashCode <= other.hashCode;
}

bool User::operator>= (const User& other) const {
    return hashCode >= other.hashCode;
}

bool User::operator== (const User& other) const {
    return hashCode == other.hashCode;
}

bool User::operator!= (const User& other) const {
    return hashCode != other.hashCode;
}