#include "../include/Train.h"
#include "../include/tools.h"

Train::Train(const std::string& _i) {
    std::strcpy(trainID, _i.c_str());
}

Train::Train(const std::string& _i, const std::string& _n, const std::string& _m,
    const std::string& _s, const std::string& _p, const std::string& _x,
    const std::string& _t, const std::string& _o, const std::string& _d,
    const std::string& _y) {
    std::strcpy(trainID, _i.c_str());
    stationNum = stringToInt(_n);
    seatNum = stringToInt(_m);
    sjtu::vector<std::string> stationName = split(_s, '|');
    for (std::size_t i = 0; i < stationName.size(); i++) {
        std::strcpy(stations[i], stationName[i].c_str());
    }
    sjtu::vector<std::string> price = split(_p, '|');
    prices[0] = 0;
    for (std::size_t i = 0; i < stationName.size() - 1; i++) {
        prices[i + 1] = prices[i] + stringToInt(price[i]);
    }
    sjtu::vector<std::string> startT = split(_x, ':');
    sjtu::vector<std::string> travelT = split(_t, '|');
    sjtu::vector<std::string> spotOverT = split(_o, '|');
    setOffTime[0] = HourMinute{stringToInt(startT[0]), stringToInt(startT[1])};
    for (int i = 1; i < stationNum - 1; i++) {
        arriveTime[i] = setOffTime[i - 1] + stringToInt(travelT[i - 1]);
        setOffTime[i] = arriveTime[i] + stringToInt(spotOverT[i - 1]);
    }
    arriveTime[stationNum - 1] = setOffTime[stationNum - 2] + stringToInt(travelT[stationNum - 2]);
    sjtu::vector<std::string> saleD = split(_d, '|');
    sjtu::vector<std::string> mm_dd;
    mm_dd = split(saleD[0], '-');
    saleDate[0] = MonthDate{stringToInt(mm_dd[0]), stringToInt(mm_dd[1])};
    mm_dd = split(saleD[1], '-');
    saleDate[1] = MonthDate{stringToInt(mm_dd[0]), stringToInt(mm_dd[1])};
    type = _y[0];
}

bool Train ::operator< (const Train& other) const {
    std::string str1(trainID, strlen(trainID));
    std::string str2(other.trainID, strlen(other.trainID));
    return str1 < str2;
}

bool Train::operator> (const Train & other) const {
    std::string str1(trainID, strlen(trainID));
    std::string str2(other.trainID, strlen(other.trainID));
    return str1 > str2;
}

bool Train ::operator<= (const Train & other) const {
    std::string str1(trainID, strlen(trainID));
    std::string str2(other.trainID, strlen(other.trainID));
    return str1 <= str2;
}

bool Train ::operator>= (const Train & other) const {
    std::string str1(trainID, strlen(trainID));
    std::string str2(other.trainID, strlen(other.trainID));
    return str1 >= str2;
}

bool Train ::operator== (const Train & other) const {
    std::string str1(trainID, strlen(trainID));
    std::string str2(other.trainID, strlen(other.trainID));
    return str1 == str2;
}

bool Train ::operator!= (const Train & other) const {
    std::string str1(trainID, strlen(trainID));
    std::string str2(other.trainID, strlen(other.trainID));
    return str1 != str2;
}