#ifndef TRAIN_H
#define TRAIN_H
#include "HourMinute.h"
#include "MonthDate.h"
#include <string>
struct Train {
    char trainID[21]{};
    int stationNum = -1;
    char stations[40][31]{};
    int seatNum = -1;
    int prices[49]{}; // 处理成前缀和
    HourMinute setOffTime[40]{};
    HourMinute arriveTime[40]{};
    MonthDate saleDate[2]{};
    char type{};
    bool release = false; // 该列车是否被发布

    int hashCode = -1;
    int ticketInfoIndex = -1; // 第一天发出的列车的余票信息在余票信息数据库中的位置

    Train() = default;

    Train(const std::string& _i);

    Train(const std::string& _i, const std::string& _n, const std::string& _m,
        const std::string& _s, const std::string& _p, const std::string& _x,
        const std::string& _t, const std::string& _o, const std::string& _d,
        const std::string& _y);

    bool operator< (const Train& other) const;

    bool operator> (const Train& other) const;

    bool operator<= (const Train& other) const;

    bool operator>= (const Train& other) const;

    bool operator== (const Train& other) const;

    bool operator!= (const Train& other) const;
};
#endif //TRAIN_H
