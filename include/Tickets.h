#ifndef TICKETS_H
#define TICKETS_H
#include<string>
#include "MonthDate.h"
#include "HourMinute.h"
struct Ticket {
    std::string trainID{};
    std::string setOffStation{};
    std::string arriveStation{};
    MonthDate setOffDate{};
    HourMinute setOffTime{};
    HourMinute arriveTime{};
    int maxSeat = -1;
    int during = -1;
    int price = -1;

    Ticket() = default;

    Ticket(const std::string& _trainID, const std::string& _setOffStation, const std::string& _arriveStation,
           const MonthDate& _setOffDate, const HourMinute& _setOffTime, const HourMinute& _arriveTime,
           const int& _price, const int& _maxSeat) {
        trainID = _trainID;
        setOffStation = _setOffStation;
        arriveStation = _arriveStation;
        setOffDate = _setOffDate;
        setOffTime = _setOffTime;
        arriveTime = _arriveTime;
        during = arriveTime - setOffTime;
        price = _price;
        maxSeat = _maxSeat;
    }

    friend std::ostream& operator<< (std::ostream& os, const Ticket& obj);
};

struct TransferTicket {
    Ticket ticket1{};
    Ticket ticket2{};
    int during = 1e9;
    int price = 1e9;

    TransferTicket() = default;

    TransferTicket(const Ticket& _ticket1, const Ticket& _ticket2) {
        ticket1 = _ticket1;
        ticket2 = _ticket2;
        during = (ticket2.setOffDate - ticket1.setOffDate) * 1440 +
                 (ticket2.setOffTime.hh - ticket1.setOffTime.hh) * 60 +
                 (ticket2.setOffTime.mm - ticket1.setOffTime.mm) + ticket2.during;
        price = ticket1.price + ticket2.price;
    }
};

inline std::ostream& operator<< (std::ostream& os, const Ticket& obj) {
    os << obj.trainID << ' ' << obj.setOffStation << ' ' << obj.setOffDate << ' ' << obj.setOffTime << " -> "
       << obj.arriveStation << ' ' << obj.setOffDate - obj.setOffTime.dd + obj.arriveTime.dd << ' '
       << obj.arriveTime << ' ' << obj.price << ' ' << obj.maxSeat;
    return os;
}
#endif //TICKETS_H
