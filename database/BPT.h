#ifndef BPT_H
#define BPT_H
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include "../include/User.h"
#include "../STLite/vector.hpp"

constexpr int node_size = 100;  //into debug mode you can modify node_size to 4

template <typename T>
class BPT {
    struct Node {
        int index = -1;
        int size = 0;
        T storage[node_size + 1]{};
        bool is_leaf = true;

        int father = -1;
        int son[node_size + 1]{};
        int prev = -1;
        int next = -1;
    };

    int root = -1;
    int head = -1;
    int new_id = 0;

    Node cur{};

    std::fstream basic_file;
    std::fstream node_file;
    std::string basic_file_name;
    std::string node_file_name;

public:
    explicit BPT(const std::string &file_name) {
        basic_file_name = "basic_" + file_name;
        node_file_name = "node_" + file_name;

        node_file.open(node_file_name, std::ios::in|std::ios::out);
        if (!node_file.is_open()) {
            node_file.open(node_file_name, std::ios::out);
            node_file.close();
            node_file.open(node_file_name, std::ios::in|std::ios::out);
        }

        basic_file.open(basic_file_name, std::ios::in|std::ios::out);
        if (!basic_file.is_open()) {
            basic_file.open(basic_file_name, std::ios::out);
            basic_file.close();
            basic_file.open(basic_file_name, std::ios::in|std::ios::out);
            root = head = new_id = 0;
            cur.index = new_id;
            writeNode(cur, cur.index);
            new_id++;
        }
        else {
            basic_file.read(reinterpret_cast<char*> (&root), sizeof(int));
            basic_file.read(reinterpret_cast<char*> (&head), sizeof(int));
            basic_file.read(reinterpret_cast<char*> (&new_id), sizeof(int));
        }
        basic_file.close();
    }

    ~BPT() {
        basic_file.open(basic_file_name, std::ios::in|std::ios::out);
        basic_file.write(reinterpret_cast<char*> (&root), sizeof(int));
        basic_file.write(reinterpret_cast<char*> (&head), sizeof(int));
        basic_file.write(reinterpret_cast<char*> (&new_id), sizeof(int));
        basic_file.close();
        node_file.close();
    }

    void readNode(const int & );

    void writeNode(Node , const int & );

    void splitNode();

    void insert(const T & , const int & );

    void remove(const T & );

    void flush(int );

    bool borrowFromRight();

    bool borrowFromLeft();

    void combine();

    int addData(const T & );

    void removeData(const T & );

    sjtu::vector<T> findData(const T& );

    void modifyData(const T&, const T& );
};
#endif //BPT_H