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
    std::string str1(station, strlen(station));
    std::string str2(other.station, strlen(other.station));
    if (str1 != str2) {return str1 < str2;}
    std::string str3(trainID, strlen(trainID));
    std::string str4(other.trainID, strlen(other.trainID));
    return str3 < str4;
}

bool StationInfo::operator> (const StationInfo& other) const {
    return !(*this < other);
}

bool StationInfo::operator== (const StationInfo& other) const {
    std::string str1(station, strlen(station));
    std::string str2(other.station, strlen(other.station));
    std::string str3(trainID, strlen(trainID));
    std::string str4(other.trainID, strlen(other.trainID));
    return str1 == str2 && str3 == str4;
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