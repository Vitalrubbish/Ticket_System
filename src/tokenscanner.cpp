#include "../include/tokenscanner.h"
#include <string>

Tokens::Tokens(const std::string &str) {
    std::size_t len = str.length();
    std::size_t pre = 0, cur = 0;
    while (str[cur] != ' ') {
        ++cur;
    }
    op_name = str.substr(pre, cur - pre);
    pre = cur + 1;
    bool flag = false;
    while (cur < len) {
        ++cur;
        if (cur == len || str[cur] == ' ') {
            std::string token = str.substr(pre, cur - pre);
            pre = cur + 1;
            if (!flag) {
                flag = true;
                if (token == "-c") {tokenType[length] = TokenType::_c;}
                if (token == "-u") {tokenType[length] = TokenType::_u;}
                if (token == "-p") {tokenType[length] = TokenType::_p;}
                if (token == "-n") {tokenType[length] = TokenType::_n;}
                if (token == "-m") {tokenType[length] = TokenType::_m;}
                if (token == "-g") {tokenType[length] = TokenType::_g;}
                if (token == "-i") {tokenType[length] = TokenType::_i;}
                if (token == "-s") {tokenType[length] = TokenType::_s;}
                if (token == "-x") {tokenType[length] = TokenType::_x;}
                if (token == "-t") {tokenType[length] = TokenType::_t;}
                if (token == "-o") {tokenType[length] = TokenType::_o;}
                if (token == "-d") {tokenType[length] = TokenType::_d;}
                if (token == "-y") {tokenType[length] = TokenType::_y;}
            }
            else {
                flag = false;
                if (tokenType[length] == TokenType::_c) {_c = token;}
                if (tokenType[length] == TokenType::_u) {_u = token;}
                if (tokenType[length] == TokenType::_p) {_p = token;}
                if (tokenType[length] == TokenType::_n) {_n = token;}
                if (tokenType[length] == TokenType::_m) {_m = token;}
                if (tokenType[length] == TokenType::_g) {_g = token;}
                if (tokenType[length] == TokenType::_i) {_i = token;}
                if (tokenType[length] == TokenType::_s) {_s = token;}
                if (tokenType[length] == TokenType::_x) {_x = token;}
                if (tokenType[length] == TokenType::_t) {_t = token;}
                if (tokenType[length] == TokenType::_o) {_o = token;}
                if (tokenType[length] == TokenType::_d) {_d = token;}
                if (tokenType[length] == TokenType::_y) {_y = token;}
                ++length;
            }
        }
    }
}