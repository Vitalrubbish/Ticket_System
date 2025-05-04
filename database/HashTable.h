#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "../STLite/vector/vector.hpp"
const int MOD = 53;
template <typename U>
struct HashTable {
    struct Pair {
        int index_{};
        U value_;

        Pair(const int& index, const U& value) {
            index_ = index;
            value_ = value;
        }

        Pair() = default;
    };

    sjtu::vector<Pair> storage[MOD]{};

    HashTable() = default;

    bool check(const int& index) {
        U ret{};
        int p = index % MOD;
        for(auto& it: storage[p]) {
            if (it.index_ == index) {
                return true;
            }
        }
        return false;
    }

    U lw(const int& index) {
        U ret{};
        int p = index % MOD;
        for(auto& it: storage[p]) {
            if (it.index_ == index) {
                ret = it.value_;
                break;
            }
        }
        return ret;
    }

    void sw(const int& index, const U& value) {
        if (check(index)) return;
        Pair newPair{index, value};
        int p = index % MOD;
        storage[p].push_back(newPair);
    }

    void erase(const int& index) {
        int p = index % MOD;
        for(std::size_t i = 0; i < storage[p].size(); i++) {
            if (storage[p][i].index_ == index) {
                storage[p][i] = storage[p][storage[p].size() - 1];
                storage[p].pop_back();
                return;
            }
        }
    }
};
#endif //HASHTABLE_H
