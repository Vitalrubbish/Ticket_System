#include <iostream>
#include <string>
#include "./include/tokenscanner.h"

int main() {
    std::string command;
    while (getline(std::cin, command)) {
        Tokens tokens{command};
        if (tokens.op_name = "")
    }
}