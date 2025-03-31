#ifndef MONTHDATE_H
#define MONTHDATE_H

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

    int operator- (const MonthDate& other) const;
};
#endif //MONTHDATE_H
