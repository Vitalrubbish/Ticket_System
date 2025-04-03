#ifndef STATIONINFO_H
#define STATIONINFO_H
#include <cstring>
#include <string>
#include "HourMinute.h"
struct StationInfo {
    char station[21]{};
    char trainID[21]{};
    HourMinute arriveTime{};
    HourMinute setOffTime{};

    StationInfo() = default;

    StationInfo(const std::string& _station);

    StationInfo(const std::string& _station, const std::string& _trainID,
                const HourMinute& _arriveTime, const HourMinute& _setOffTime);

    bool operator< (const StationInfo& other) const;

    bool operator> (const StationInfo& other) const;

    bool operator== (const StationInfo& other) const;

    bool operator!= (const StationInfo& other) const;

    bool operator<= (const StationInfo& other) const;

    bool operator>= (const StationInfo& other) const;
};
#endif //STATIONINFO_H
