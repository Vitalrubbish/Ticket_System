#ifndef MONTHDATE_H
#define MONTHDATE_H
#include <iostream>

struct MonthDate {
    int mm;
    int dd;

    MonthDate() = default;

    MonthDate(const int &_mm, const int &_dd);

    bool operator< (const MonthDate& other) const;

    bool operator> (const MonthDate& other) const;

    bool operator== (const MonthDate& other) const;

    bool operator!= (const MonthDate& other) const;

    bool operator<= (const MonthDate& other) const;

    bool operator>= (const MonthDate& other) const;

    MonthDate operator- (int d) const;

    MonthDate operator+ (int d) const;

    int operator- (const MonthDate& other) const;

    friend std::ostream& operator<<(std::ostream& os, const MonthDate& obj);
};
#endif //MONTHDATE_H
