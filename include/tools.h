#ifndef TOOLS_H
#define TOOLS_H
#include <string>
#include "../STLite/vector/vector.hpp"
#include "Tickets.h"

inline int stringToInt(const std::string& str) {
    int ans = 0, sign = 1;
    std::size_t pos = 0;
    if (str[pos] == '-') {
        ++pos;
        sign = -1;
    }
    while (pos < str.size()) {
        ans = ans * 10 + str[pos++] - '0';
    }
    return ans * sign;
}

inline sjtu::vector<std::string> split(const std::string& str,const char& c) {
    sjtu::vector<std::string> ret;
    std::size_t pre = 0;
    for (std::size_t i = 0; i < str.size(); i++) {
        if (str[i] == c || i == str.size() - 1) {
            ret.push_back(str.substr(pre, i - pre));
            pre = i + 1;
        }
    }
    return ret;
}

inline Ticket a[10001], tmp[10001];

inline void mergeSortByTime(int l, int r) {
    if (r - l <= 0) {
        return;
    }
    int mid = (l + r) / 2;
    mergeSortByTime(l, mid);
    mergeSortByTime(mid + 1, r);
    for (int i = l; i <= r; i++) {
        tmp[i] = a[i];
    }
    int p = l, q = mid + 1, cursor = l;
    while (cursor <= r) {
        if (p <= mid && q <= r) {
            if (tmp[p].during < tmp[q].during) {
                a[cursor++] = tmp[p++];
            }
            else {
                a[cursor++] = tmp[q++];
            }
        }
        else if (p <= mid) {
            a[cursor++] = tmp[p++];
        }
        else {
            a[cursor++] = tmp[q++];
        }
    }
}

inline void mergeSortByCost(int l, int r) {
    if (r - l <= 0) {
        return;
    }
    int mid = (l + r) / 2;
    mergeSortByTime(l, mid);
    mergeSortByTime(mid + 1, r);
    for (int i = l; i <= r; i++) {
        tmp[i] = a[i];
    }
    int p = l, q = mid + 1, cursor = l;
    while (cursor <= r) {
        if (p <= mid && q <= r) {
            if (tmp[p].price < tmp[q].price) {
                a[cursor++] = tmp[p++];
            }
            else {
                a[cursor++] = tmp[q++];
            }
        }
        else if (p <= mid) {
            a[cursor++] = tmp[p++];
        }
        else {
            a[cursor++] = tmp[q++];
        }
    }
}

inline sjtu::vector<Ticket> sortByTime(const sjtu::vector<Ticket>& vec) {
    int len = static_cast<int>(vec.size());
    sjtu::vector<Ticket> ret;
    for (int i = 0; i < len; i++) {a[i] = vec[i];}
    mergeSortByTime(0, len - 1);
    for (int i = 0; i < len; i++) {ret.push_back(a[i]);}
    return ret;
}

inline sjtu::vector<Ticket> sortByCost(const sjtu::vector<Ticket>& vec) {
    int len = static_cast<int>(vec.size());
    sjtu::vector<Ticket> ret;
    for (int i = 0; i < len; i++) {a[i] = vec[i];}
    mergeSortByCost(0, len - 1);
    for (int i = 0; i < len; i++) {ret.push_back(a[i]);}
    return ret;
}
#endif //TOOLS_H