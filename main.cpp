#include <iostream>
#include <string>
#include <fstream>
#include "./include/tokenscanner.h"
#include "./include/User.h"
#include "./STLite/vector.hpp"
#include "./include/tools.h"

BPT<User> userBPT("user");

std::fstream system_file;
extern int newTicketInfoIndex = 0;
extern int newOrderIndex = 0;
bool setUp = true;

void initialise() {
    system_file.open("system_file", std::ios::in|std::ios::out);
    if (!system_file.is_open()) {
        system_file.open("system_file", std::ios::out);
        system_file.close();
        setUp = false;
    }
    else {
        system_file.read(reinterpret_cast<char*>(&newTicketInfoIndex), sizeof(int));
        system_file.read(reinterpret_cast<char*>(&newOrderIndex), sizeof(int));
        system_file.close();
    }
}

void flush() {
    system_file.open("system_file", std::ios::in|std::ios::out);
    system_file.write(reinterpret_cast<char*>(&newTicketInfoIndex), sizeof(int));
    system_file.write(reinterpret_cast<char*>(&newOrderIndex), sizeof(int));
    system_file.close();
}

int main() {
    initialise();
    std::string command;
    while (getline(std::cin, command)) {
        Tokens tokens{command};
        std::cout << tokens.timeSlot << " ";
        if (tokens.op_name == "exit") {
            std::cout << "bye\n";
            break;
        }
        if (tokens.op_name == "add_user") {
            User newUser{tokens._u, tokens._p, tokens._n, tokens._m, tokens._g};
            if (!setUp) {
                std::cout << userBPT.addData(newUser) << '\n';
                setUp = true;
            }
            else {
                sjtu::vector<User> vec = userBPT.findData(User{tokens._c});
                if (!vec.empty()) {
                    if (vec[0].privilege <= newUser.privilege || !vec[0].login) {
                        std::cout << -1 << '\n';
                    }
                    else {
                        std::cout << userBPT.addData(newUser) << '\n';
                    }
                }
            }
        }
        if (tokens.op_name == "login") {
            sjtu::vector<User> vec = userBPT.findData(User{tokens._u});
            if (!vec.empty() && !vec[0].login) {
                std::string pwd(vec[0].password, strlen(vec[0].password));
                if (pwd == tokens._p) {
                    vec[0].login = true;
                    userBPT.modifyData(User{tokens._u}, vec[0]);
                    std::cout << 0 << '\n';
                }
                else {
                    std::cout << -1 << '\n';
                }
            }
            else {
                std::cout << -1 << '\n';
            }
        }
        if (tokens.op_name == "logout") {
            sjtu::vector<User> vec = userBPT.findData(User{tokens._u});
            if (!vec.empty() && vec[0].login) {
                vec[0].login = false;
                userBPT.modifyData(User{tokens._u}, vec[0]);
                std::cout << 0 << '\n';
            }
            else {
                std::cout << -1 << '\n';
            }
        }
        if (tokens.op_name == "query_profile") {
            sjtu::vector<User> vec1 = userBPT.findData(User{tokens._c});
            sjtu::vector<User> vec2 = userBPT.findData(User{tokens._u});
            if (!vec1.empty() && !vec2.empty() && vec1[0].login) {
                if (vec1[0].privilege > vec2[0].privilege || vec1[0] == vec2[0]) {
                    std::cout << vec2[0].username << ' '
                              << vec2[0].name << ' '
                              << vec2[0].mailAddr << ' '
                              << vec2[0].privilege << '\n';
                }
                else {
                    std::cout << -1 << '\n';
                }
            }
            else {
                std::cout << -1 << '\n';
            }
        }
        if (tokens.op_name == "modify_profile") {
            sjtu::vector<User> vec1 = userBPT.findData(User{tokens._c});
            sjtu::vector<User> vec2 = userBPT.findData(User{tokens._u});
            if (!vec1.empty() && !vec2.empty() && vec1[0].login) {
                if (vec1[0].privilege > vec2[0].privilege || vec1[0] == vec2[0]) {
                    if (!tokens._p.empty()) {std::strcpy(vec2[0].password, tokens._p.c_str());}
                    if (!tokens._n.empty()) {std::strcpy(vec2[0].name, tokens._n.c_str());}
                    if (!tokens._m.empty()) {std::strcpy(vec2[0].mailAddr, tokens._m.c_str());}
                    if (!tokens._g.empty()) {vec2[0].privilege = stringToInt(tokens._g);}
                    userBPT.modifyData(User{tokens._u}, vec2[0]);
                    std::cout << vec2[0].username << ' '
                              << vec2[0].name << ' '
                              << vec2[0].mailAddr << ' '
                              << vec2[0].privilege << '\n';
                }
                else {
                    std::cout << -1 << '\n';
                }
            }
            else {
                std::cout << -1 << '\n';
            }
        }
    }
    flush();
}