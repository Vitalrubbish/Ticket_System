#include "../include/HourMinute.h"

HourMinute::HourMinute(const int& _hh, const int& _mm) {
    dd = 0;
    hh = _hh;
    mm = _mm;
}

HourMinute::HourMinute(const int& _dd, const int& _hh, const int& _mm) {
    dd = _dd;
    hh = _hh;
    mm = _mm;
}

HourMinute &HourMinute::operator=(const HourMinute& other) {
    if (this == &other) {
        return *this;
    }
    dd = other.dd;
    hh = other.hh;
    mm = other.mm;
    return *this;
}

bool HourMinute::operator< (const HourMinute &other) const {
    if (hh != other.hh) {
        return hh < other.hh;
    }
    return mm < other.mm;
}

bool HourMinute::operator> (const HourMinute &other) const {
    if (hh != other.hh) {
        return hh > other.hh;
    }
    return mm > other.mm;
}

bool HourMinute::operator== (const HourMinute &other) const {
    return hh == other.hh && mm == other.mm;
}

bool HourMinute::operator!= (const HourMinute &other) const {
    return !(*this == other);
}

bool HourMinute::operator<= (const HourMinute &other) const {
    return *this < other || *this == other;
}

bool HourMinute::operator>= (const HourMinute &other) const {
    return *this > other || *this == other;
}

HourMinute HourMinute::operator+ (const int &t) const {
    HourMinute ret = *this;
    ret.mm += t;
    ret.hh += ret.mm / 60;
    ret.mm %= 60;
    ret.dd += ret.hh / 24;
    ret.hh %= 24;
    return ret;
}
