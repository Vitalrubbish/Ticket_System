#ifndef HOURMINUTE_H
#define HOURMINUTE_H
#include <iostream>

struct HourMinute {
    int dd; // 存储当前日期和列车始发日期的差值
    int hh;
    int mm;

    HourMinute() = default;

    HourMinute(const int& _hh, const int& _mm);

    HourMinute(const int& _dd, const int& _hh, const int& _mm);

    HourMinute &operator= (const HourMinute &other);

    bool operator< (const HourMinute &other) const;

    bool operator> (const HourMinute &other) const;

    bool operator== (const HourMinute &other) const;

    bool operator!= (const HourMinute &other) const;

    bool operator<= (const HourMinute &other) const;

    bool operator>= (const HourMinute &other) const;

    HourMinute operator+ (const int &t) const;

    int operator- (const HourMinute& other) const;

    friend std::ostream& operator<< (std::ostream& os, const HourMinute& obj);
};
#endif //HOURMINUTE_H
