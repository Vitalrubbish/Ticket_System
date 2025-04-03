#include "../include/MonthDate.h"

int days[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

MonthDate::MonthDate(const int& _mm, const int& _dd) {
    mm = _mm;
    dd = _dd;
}

bool MonthDate::operator< (const MonthDate& other) const {
    if (mm != other.mm) {
        return mm < other.mm;
    }
    return dd < other.dd;
}

bool MonthDate::operator> (const MonthDate& other) const {
    if (mm != other.mm) {
        return mm > other.mm;
    }
    return dd > other.dd;
}

bool MonthDate::operator== (const MonthDate& other) const {
    return mm == other.mm && dd == other.dd;
}

bool MonthDate::operator!= (const MonthDate& other) const {
    return !(*this == other);
}

bool MonthDate::operator<= (const MonthDate& other) const {
    return *this == other || *this < other;
}

bool MonthDate::operator>= (const MonthDate& other) const {
    return *this == other || *this > other;
}

MonthDate MonthDate::operator- (int d) const {
    MonthDate ret = *this;
    while (d >= ret.dd) {
        d -= ret.dd;
        ret.mm--;
        ret.dd = days[ret.mm];
    }
    ret.dd -= d;
    return ret;
}

MonthDate MonthDate::operator+ (int d) const {
    MonthDate ret = *this;
    ret.dd += d;
    while (ret.dd > days[ret.mm]) {
        ret.dd -= days[ret.mm];
        ret.mm++;
    }
    return ret;
}

int MonthDate::operator- (const MonthDate& other) const {
    int ret = 0;
    if (mm == other.mm) {
        ret = dd - other.dd;
    }
    else {
        ret += days[other.mm] - other.dd + dd;
        for (int i = other.mm + 1; i <= mm - 1; i++) {
            ret += days[i];
        }
    }
    return ret;
}

std::ostream& operator<<(std::ostream& os, const MonthDate& obj) {
    if (obj.mm < 10) {os << 0;}
    os << obj.mm << '-';
    if (obj.dd < 10) {os << 0;}
    os << obj.dd;
    return os;
}
