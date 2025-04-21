#include "../include/Order.h"
Order::Order(const std::string& _username) {
    std::strcpy(username, _username.c_str());
}

Order::Order(const std::string& _username,  const std::string& _trainID, const std::string& _setOffStation,
      const std::string& _arriveStation, MonthDate _setOffDate, HourMinute _setOffTime, HourMinute _arriveTime) {
    std::strcpy(username, _username.c_str());
    std::strcpy(trainID, _trainID.c_str());
    std::strcpy(setOffStation, _setOffStation.c_str());
    std::strcpy(arriveStation, _arriveStation.c_str());
    setOffDate = _setOffDate;
    setOffTime = _setOffTime;
    arriveTime = _arriveTime;
}

bool Order::operator< (const Order& other) const {
    std::string str1(username);
    std::string str2(other.username);
    if (str1 != str2) {return str1 < str2;}
    return orderIndex > other.orderIndex;
}

bool Order::operator> (const Order& other) const {
    std::string str1(username);
    std::string str2(other.username);
    if (str1 != str2) {return str1 > str2;}
    return orderIndex < other.orderIndex;
}

bool Order::operator== (const Order& other) const {
    std::string str1(username);
    std::string str2(other.username);
    return str1 == str2 && orderIndex == other.orderIndex;
}

bool Order::operator!= (const Order& other) const {
    return !(*this == other);
}

bool Order::operator<= (const Order& other) const {
    return *this == other || *this < other;
}

bool Order::operator>= (const Order& other) const {
    return *this == other || *this > other;
}

std::ostream& operator<< (std::ostream& os, const Order& obj) {
    if (obj.orderStatus == OrderStatus::success) {os << "[success] ";}
    if (obj.orderStatus == OrderStatus::pending) {os << "[pending] ";}
    if (obj.orderStatus == OrderStatus::refunded) {os << "[refunded] ";}
    os << obj.trainID << ' ' << obj.setOffStation << ' ' << obj.setOffDate << ' '
       << obj.setOffTime << " -> " << obj.arriveStation << ' '
       << obj.setOffDate - obj.setOffTime.dd + obj.arriveTime.dd << ' ' << obj.arriveTime << ' '
       << obj.price << ' ' << obj.seats;
    return os;
}

PendingOrder::PendingOrder(const std::string& _trainID) {
    std::strcpy(trainID, _trainID.c_str());
}

PendingOrder::PendingOrder(const std::string& _username,  const std::string& _trainID, const std::string& _setOffStation,
      const std::string& _arriveStation, MonthDate _setOffDate) {
    std::strcpy(username, _username.c_str());
    std::strcpy(trainID, _trainID.c_str());
    std::strcpy(setOffStation, _setOffStation.c_str());
    std::strcpy(arriveStation, _arriveStation.c_str());
    setOffDate = _setOffDate;
}

bool PendingOrder::operator< (const PendingOrder& other) const {
    std::string str1(trainID);
    std::string str2(other.trainID);
    if (str1 != str2) {return str1 < str2;}
    return orderIndex < other.orderIndex;
}

bool PendingOrder::operator> (const PendingOrder& other) const {
    std::string str1(trainID);
    std::string str2(other.trainID);
    if (str1 != str2) {return str1 > str2;}
    return orderIndex > other.orderIndex;
}

bool PendingOrder::operator== (const PendingOrder& other) const {
    std::string str1(trainID);
    std::string str2(other.trainID);
    return str1 == str2 && orderIndex == other.orderIndex;
}

bool PendingOrder::operator!= (const PendingOrder& other) const {
    return !(*this == other);
}
bool PendingOrder::operator<= (const PendingOrder& other) const {
    return *this == other || *this < other;
}

bool PendingOrder::operator>= (const PendingOrder& other) const {
    return *this == other || *this > other;
}

std::ostream& operator<< (std::ostream& os, const PendingOrder& obj) {
    os << obj.trainID << ' ' << obj.setOffStation << ' ' << obj.setOffDate
       << ' ' << obj.arriveStation << ' ' <<  obj.seats;
    return os;
}