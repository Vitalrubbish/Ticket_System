#ifndef ORDER_H
#define ORDER_H
#include <string>
#include <cstring>
#include "HourMinute.h"
#include "MonthDate.h"
enum class OrderStatus {success, pending, refunded};
struct Order {
    char username[21]{};
    char trainID[21]{};
    char setOffStation[31]{};
    char arriveStation[31]{};
    MonthDate setOffDate{};
    HourMinute setOffTime{};
    HourMinute arriveTime{};
    int price = -1;
    int seats = -1;
    int orderIndex = -1;
    OrderStatus orderStatus{};

    Order() = default;

    explicit Order(const std::string& _username);

    Order(const std::string& _username,  const std::string& _trainID, const std::string& _setOffStation,
          const std::string& _arriveStation, MonthDate setOffDate, HourMinute setOffTime, HourMinute arriveTime);

    bool operator< (const Order& other) const;

    bool operator> (const Order& other) const;

    bool operator== (const Order& other) const;

    bool operator!= (const Order& other) const;

    bool operator<= (const Order& other) const;

    bool operator>= (const Order& other) const;

    friend std::ostream& operator<< (std::ostream& os, const Order& obj);
};

struct PendingOrder {
    char username[21]{};
    char trainID[21]{};
    char setOffStation[21]{};
    char arriveStation[21]{};
    MonthDate setOffDate{};
    int seats = -1;
    int orderIndex = -1;

    PendingOrder() = default;

    explicit PendingOrder(const std::string& _trainID);

    PendingOrder(const std::string& _username,  const std::string& _trainID, const std::string& _setOffStation,
          const std::string& _arriveStation, MonthDate setOffDate);

    bool operator< (const PendingOrder& other) const;

    bool operator> (const PendingOrder& other) const;

    bool operator== (const PendingOrder& other) const;

    bool operator!= (const PendingOrder& other) const;

    bool operator<= (const PendingOrder& other) const;

    bool operator>= (const PendingOrder& other) const;
};
#endif //ORDER_H
