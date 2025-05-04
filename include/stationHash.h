#ifndef STATIONHASH_H
#define STATIONHASH_H
#include <string>
#include <cstring>
#include <fstream>
const int MOD_ = 1e6 + 3;
const int pr = 31;
struct StationHash {
    std::fstream file;
    std::fstream state_file;
    int state[MOD_] = {};

    explicit StationHash(const std::string& file_name) {
        file.open(file_name, std::ios::in|std::ios::out);
        if (!file.is_open()) {
            file.open(file_name, std::ios::out);
            file.close();
            file.open(file_name, std::ios::in|std::ios::out);
            file.seekp(MOD_ * 31 - 1);
            file.write("", 1);
        }
        state_file.open("state_" + file_name, std::ios::in|std::ios::out);
        if (!state_file.is_open()) {
            state_file.open("state_" + file_name, std::ios::out);
            state_file.close();
            state_file.open("state_" + file_name, std::ios::in|std::ios::out);
        }
        else {
            state_file.read(reinterpret_cast<char*>(state), MOD_ * sizeof(int));
        }
    }

    ~StationHash() {
        state_file.seekp(0);
        state_file.write(reinterpret_cast<char*>(state), MOD_ * sizeof(int));
        state_file.close();
        file.close();
    }

    int queryHash(const std::string& str) {
        char str_[31];
        std::strcpy(str_, str.c_str());
        int ans = 0;
        for (auto& i: str) {
            ans = (ans * pr + i) % MOD_;
        }
        ans = (ans + MOD_) % MOD_;
        while (state[ans] == 1) {
            if (get(ans) == str) {return ans;}
            ans = (ans + 1) % MOD_;
        }
        state[ans] = 1;
        file.seekp(ans * 31);
        file.write(str_, 31);
        return ans;
    }

    std::string get(int hashCode) {
        char str_[31];
        file.seekp(hashCode * 31);
        file.read(str_, 31);
        return std::string{str_};
    }
};
#endif //STATIONHASH_H
