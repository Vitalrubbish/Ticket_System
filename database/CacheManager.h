#ifndef CACHEMANAGER_H
#define CACHEMANAGER_H
#include "../STLite/list/list.hpp"
#include "../STLite/vector/vector.hpp"
#include "./HashTable.h"
const int max_size_ = 50;
template <typename T>
struct CacheManager {
    struct LRUNode {
        int index;
    };
    HashTable<T> hashTable;
    HashTable<typename sjtu::list<LRUNode>::iterator> position;
    sjtu::list<LRUNode> lis;

    CacheManager() = default;

    [[nodiscard]] size_t size() {
        return lis.size();
    }

    bool checkExist(const int& index) {
        return hashTable.check(index);
    }

    T get(const int& index) {
        typename sjtu::list<LRUNode>::iterator pos = position.lw(index);
        LRUNode newNode{index};
        lis.erase(pos);
        position.erase(index);
        position.sw(index, lis.push_back(newNode));
        return hashTable.lw(index);
    }

    void put(const int& index, const T& val) {
        LRUNode newNode{index};
        if (checkExist(index)) {
            typename sjtu::list<LRUNode>::iterator pos = position.lw(index);
            lis.erase(pos);
            position.erase(index);
            hashTable.erase(index);
        }
        hashTable.sw(index, val);
        position.sw(index, lis.push_back(newNode));
    }
};
#endif //CACHEMANAGER_H
