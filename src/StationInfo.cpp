#include "../include/StationInfo.h"
StationInfo::StationInfo(const std::string& _station) {
    std::strcpy(station, _station.c_str());
}

StationInfo::StationInfo(const std::string& _station, const std::string& _trainID,
                         const HourMinute& _arriveTime, const HourMinute& _setOffTime) {
    std::strcpy(station, _station.c_str());
    std::strcpy(trainID, _trainID.c_str());
    arriveTime = _arriveTime;
    setOffTime = _setOffTime;
}

bool StationInfo::operator< (const StationInfo& other) const {
    int val = strcmp(station, other.station);
    if (val != 0) {return val < 0;}
    val = strcmp(trainID, other.trainID);
    return val < 0;
}

bool StationInfo::operator> (const StationInfo& other) const {
    return !(*this < other);
}

bool StationInfo::operator== (const StationInfo& other) const {
    return strcmp(station, other.station) == 0 && strcmp(trainID, other.trainID) == 0;
}

bool StationInfo::operator!= (const StationInfo& other) const {
    return !(*this == other);
}

bool StationInfo::operator<= (const StationInfo& other) const {
    return *this < other || *this == other;
}

bool StationInfo::operator>= (const StationInfo& other) const {
    return *this > other || *this == other;
}