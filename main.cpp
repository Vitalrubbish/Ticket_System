#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include "./include/tokenscanner.h"
#include "./include/User.h"
#include "./include/Train.h"
#include "./include/tools.h"
#include "./include/TicketInfo.h"
#include "./include/StationInfo.h"
#include "./include/Tickets.h"
#include "./include/Order.h"
#include "./STLite/vector/vector.hpp"
#include "./STLite/map/map.hpp"

BPT<User> userBPT("user");
BPT<Train> trainBPT("train");
BPT<StationInfo> stationBPT("station");
BPT<Order> orderBPT("order");
BPT<PendingOrder> pendingOrderBPT("pendingOrder");
TicketInfo ticketInfo{};

std::fstream system_file;
int newTicketInfoIndex = 0;
int newOrderIndex = 0;
bool setUp = true;

void initialise() {
    system_file.open("system_file", std::ios::in|std::ios::out);
    if (!system_file.is_open()) {
        system_file.open("system_file", std::ios::out);
        system_file.close();
        setUp = false;
    }
    else {
        system_file.read(reinterpret_cast<char*>(&newTicketInfoIndex), sizeof(int));
        system_file.read(reinterpret_cast<char*>(&newOrderIndex), sizeof(int));
        system_file.close();
    }
}

void flush() {
    system_file.open("system_file", std::ios::in|std::ios::out);
    system_file.write(reinterpret_cast<char*>(&newTicketInfoIndex), sizeof(int));
    system_file.write(reinterpret_cast<char*>(&newOrderIndex), sizeof(int));
    system_file.close();
}

sjtu::vector<Ticket> queryTicket(const std::string&, const std::string&, const std::string&);
sjtu::vector<Ticket> queryNextTicket(const std::string&, const std::string&, const std::string&, const Ticket&);

int main() {
    //freopen("output.txt", "w", stdout);
    initialise();
    std::string command;
    while (getline(std::cin, command)) {
        Tokens tokens{command};
        std::cout << tokens.timeSlot << " ";
        if (tokens.op_name == "exit") {
            userBPT.logout();
            std::cout << "bye\n";
            break;
        }
        if (tokens.op_name == "add_user") {
            User newUser{tokens._u, tokens._p, tokens._n, tokens._m, tokens._g};
            if (!setUp) {
                std::cout << userBPT.addData(newUser) << '\n';
                setUp = true;
            }
            else {
                sjtu::vector<User> vec = userBPT.findData(User{tokens._c});
                if (!vec.empty()) {
                    if (vec[0].privilege <= newUser.privilege || !vec[0].login) {
                        std::cout << -1 << '\n';
                    }
                    else {
                        std::cout << userBPT.addData(newUser) << '\n';
                    }
                }
                else {
                    std::cout << -1 << '\n';
                }
            }
        }
        if (tokens.op_name == "login") {
            sjtu::vector<User> vec = userBPT.findData(User{tokens._u});
            if (!vec.empty() && !vec[0].login) {
                std::string pwd(vec[0].password, strlen(vec[0].password));
                if (pwd == tokens._p) {
                    vec[0].login = true;
                    userBPT.modifyData(User{tokens._u}, vec[0]);
                    std::cout << 0 << '\n';
                }
                else {
                    std::cout << -1 << '\n';
                }
            }
            else {
                std::cout << -1 << '\n';
            }
        }
        if (tokens.op_name == "logout") {
            sjtu::vector<User> vec = userBPT.findData(User{tokens._u});
            if (!vec.empty() && vec[0].login) {
                vec[0].login = false;
                userBPT.modifyData(User{tokens._u}, vec[0]);
                std::cout << 0 << '\n';
            }
            else {
                std::cout << -1 << '\n';
            }
        }
        if (tokens.op_name == "query_profile") {
            sjtu::vector<User> vec1 = userBPT.findData(User{tokens._c});
            sjtu::vector<User> vec2 = userBPT.findData(User{tokens._u});
            if (!vec1.empty() && !vec2.empty() && vec1[0].login) {
                if (vec1[0].privilege > vec2[0].privilege || vec1[0] == vec2[0]) {
                    std::cout << vec2[0].username << ' '
                              << vec2[0].name << ' '
                              << vec2[0].mailAddr << ' '
                              << vec2[0].privilege << '\n';
                }
                else {
                    std::cout << -1 << '\n';
                }
            }
            else {
                std::cout << -1 << '\n';
            }
        }
        if (tokens.op_name == "modify_profile") {
            sjtu::vector<User> vec1 = userBPT.findData(User{tokens._c});
            sjtu::vector<User> vec2 = userBPT.findData(User{tokens._u});
            if (!vec1.empty() && !vec2.empty() && vec1[0].login) {
                if ((tokens._g.empty() || stringToInt(tokens._g) < vec1[0].privilege) && (vec1[0].privilege > vec2[0].privilege || vec1[0] == vec2[0])) {
                    if (!tokens._p.empty()) {std::strcpy(vec2[0].password, tokens._p.c_str());}
                    if (!tokens._n.empty()) {std::strcpy(vec2[0].name, tokens._n.c_str());}
                    if (!tokens._m.empty()) {std::strcpy(vec2[0].mailAddr, tokens._m.c_str());}
                    if (!tokens._g.empty()) {vec2[0].privilege = stringToInt(tokens._g);}
                    userBPT.modifyData(User{tokens._u}, vec2[0]);
                    std::cout << vec2[0].username << ' '
                              << vec2[0].name << ' '
                              << vec2[0].mailAddr << ' '
                              << vec2[0].privilege << '\n';
                }
                else {
                    std::cout << -1 << '\n';
                }
            }
            else {
                std::cout << -1 << '\n';
            }
        }
        if (tokens.op_name == "add_train") {
            sjtu::vector<Train> vec = trainBPT.findData(Train{tokens._i});
            if (vec.empty()) {
                Train newTrain{tokens._i, tokens._n, tokens._m, tokens._s, tokens._p,
                    tokens._x, tokens._t, tokens._o, tokens._d, tokens._y};
                trainBPT.addData(newTrain);
                std::cout << 0 << '\n';
            }
            else {
                std::cout << -1 << '\n';
            }
        }
        if (tokens.op_name == "delete_train") {
            sjtu::vector<Train> vec = trainBPT.findData(Train{tokens._i});
            if (!vec.empty()) {
                if (!vec[0].release) {
                    trainBPT.removeData(vec[0]);
                    std::cout << 0 << '\n';
                }
                else {
                    std::cout << -1 << '\n';
                }
            }
            else {
                std::cout << -1 << '\n';
            }
        }
        if (tokens.op_name == "release_train") {
            sjtu::vector<Train> vec = trainBPT.findData(Train{tokens._i});
            if (!vec.empty()) {
                if (!vec[0].release) {
                    vec[0].release = true;
                    int len = (vec[0].saleDate[1] - vec[0].saleDate[0] + 1) * vec[0].stationNum;
                    vec[0].ticketInfoIndex = newTicketInfoIndex;
                    newTicketInfoIndex += len;
                    trainBPT.modifyData(Train{tokens._i}, vec[0]);
                    for (int i = 0; i < vec[0].stationNum; i++) {
                        std::string stationN(vec[0].stations[i]);
                        StationInfo newStationInfo{stationN, tokens._i, vec[0].arriveTime[i], vec[0].setOffTime[i]};
                        stationBPT.addData(newStationInfo);
                    }
                    sjtu::vector<int> soldTicket;
                    for (int i = 0; i < vec[0].stationNum; i++) {soldTicket.push_back(0);}
                    for (int i = 0; i < vec[0].saleDate[1] - vec[0].saleDate[0] + 1; i++) {
                        ticketInfo.writeTicketInfo(vec[0].ticketInfoIndex + i * vec[0].stationNum, soldTicket);
                    }
                    std::cout << 0 << '\n';
                }
                else {
                    std::cout << -1 << '\n';
                }
            }
            else {
                std::cout << -1 << '\n';
            }
        }
        if (tokens.op_name == "query_train") {
            sjtu::vector<Train> vec = trainBPT.findData(Train{tokens._i});
            if (!vec.empty()) {
                sjtu::vector<std::string> mm_dd = split(tokens._d, '-');
                MonthDate md{stringToInt(mm_dd[0]), stringToInt(mm_dd[1])};
                if (vec[0].saleDate[0] <= md && md <= vec[0].saleDate[1]) {
                    std::cout << vec[0].trainID << ' ' << vec[0].type << '\n';
                    if (vec[0].release) {
                        sjtu::vector<int> soldT = ticketInfo.readTicketInfo(vec[0].ticketInfoIndex + vec[0].stationNum * (md - vec[0].saleDate[0]), vec[0].stationNum);
                        std::cout << vec[0].stations[0] << " xx-xx xx:xx -> " << md << ' '
                                  << vec[0].setOffTime[0] << ' ' << vec[0].prices[0] << ' ' << vec[0].seatNum - soldT[0] << '\n';
                        for (int i = 1; i < vec[0].stationNum - 1; i++) {
                            std::cout << vec[0].stations[i] << ' ' << md + vec[0].arriveTime[i].dd << ' ' << vec[0].arriveTime[i] << " -> "
                                      << md + vec[0].setOffTime[i].dd << ' ' << vec[0].setOffTime[i] << ' ' << vec[0].prices[i] << ' '
                                      << vec[0].seatNum - soldT[i] << '\n';
                        }
                        std::cout << vec[0].stations[vec[0].stationNum - 1] << ' ' << md + vec[0].arriveTime[vec[0].stationNum - 1].dd << ' ' << vec[0].arriveTime[vec[0].stationNum - 1]
                                  << " -> xx-xx xx:xx " << vec[0].prices[vec[0].stationNum - 1] << " x\n";
                    }
                    else {
                        std::cout << vec[0].stations[0] << " xx-xx xx:xx -> " << md << ' '
                                  << vec[0].setOffTime[0] << ' ' << vec[0].prices[0] << ' ' << vec[0].seatNum << '\n';
                        for (int i = 1; i < vec[0].stationNum - 1; i++) {
                            std::cout << vec[0].stations[i] << ' ' << md + vec[0].arriveTime[i].dd << ' ' << vec[0].arriveTime[i] << " -> "
                                      << md + vec[0].setOffTime[i].dd << ' ' << vec[0].setOffTime[i] << ' ' << vec[0].prices[i] << ' '
                                      << vec[0].seatNum << '\n';
                        }
                        std::cout << vec[0].stations[vec[0].stationNum - 1] << ' ' << md + vec[0].arriveTime[vec[0].stationNum - 1].dd << ' ' << vec[0].arriveTime[vec[0].stationNum - 1]
                                  << " -> xx-xx xx:xx " << vec[0].prices[vec[0].stationNum - 1] << " x\n";
                    }
                }
                else {
                    std::cout << -1 << '\n';
                }
            }
            else {
                std::cout << -1 << '\n';
            }
        }
        if (tokens.op_name == "query_ticket") {
            sjtu::vector<Ticket> vec = queryTicket(tokens._s, tokens._t, tokens._d);
            std::cout << vec.size() << '\n';
            if (tokens._p == "cost") {vec = sortByCost(vec);}
            else {vec = sortByTime(vec);}
            for (auto & i : vec) {
                std::cout << i << '\n';
            }
        }
        if (tokens.op_name == "query_transfer") {
            sjtu::map<std::string, int> transferS;
            sjtu::vector<StationInfo> vec1 = stationBPT.findData(StationInfo{tokens._s});
            sjtu::vector<StationInfo> vec2 = stationBPT.findData(StationInfo{tokens._t});
            sjtu::vector<std::string> transferStation;
            for (auto & i : vec1) {
                std::string _trainID(i.trainID);
                sjtu::vector<Train> trainV = trainBPT.findData(Train{_trainID});
                for (int j = trainV[0].stationNum - 1; j >= 0; j--) {
                    if (strcmp(trainV[0].stations[j], tokens._s.c_str()) == 0) {break;}
                    std::string _station(trainV[0].stations[j]);
                    if (!transferS.count(_station)) {transferS[_station] = 1;}
                }
            }
            for (auto & i : vec2) {
                std::string _trainID(i.trainID);
                sjtu::vector<Train> trainV = trainBPT.findData(Train{_trainID});
                for (int j = 0; j < trainV[0].stationNum - 1; j++) {
                    if (strcmp(trainV[0].stations[j], tokens._t.c_str()) == 0) {break;}
                    std::string _station(trainV[0].stations[j]);
                    if (transferS.count(_station) && transferS[_station] != 0) {
                        transferStation.push_back(_station);
                        transferS[_station] = 0;
                    }
                }
            }

            TransferTicket bestTicket{};
            for (const auto & i : transferStation) {
                sjtu::vector<Ticket> ticketSet1 = queryTicket(tokens._s, i, tokens._d);
                for (const auto & j : ticketSet1) {
                    sjtu::vector<Ticket> ticketSet2 = queryNextTicket(i, tokens._t, tokens._d, j);
                    for (const auto & k : ticketSet2){
                        TransferTicket curTicket{j, k};
                        if (tokens._p == "time") {
                            if (curTicket.during < bestTicket.during) {
                                bestTicket = curTicket;
                            }
                            else if (curTicket.during == bestTicket.during && curTicket.price < bestTicket.price) {
                                bestTicket = curTicket;
                            }
                            else if (curTicket.during == bestTicket.during && curTicket.price == bestTicket.price && curTicket.ticket1.trainID < bestTicket.ticket1.trainID) {
                                bestTicket = curTicket;
                            }
                            else if (curTicket.during == bestTicket.during && curTicket.price == bestTicket.price && curTicket.ticket1.trainID == bestTicket.ticket1.trainID && curTicket.ticket2.trainID < bestTicket.ticket2.trainID) {
                                bestTicket = curTicket;
                            }
                        }
                        if (tokens._p == "cost") {
                            if (curTicket.price < bestTicket.price) {
                                bestTicket = curTicket;
                            }
                            else if (curTicket.price == bestTicket.price && curTicket.during < bestTicket.during) {
                                bestTicket = curTicket;
                            }
                            else if (curTicket.price == bestTicket.price && curTicket.during == bestTicket.during && curTicket.ticket1.trainID < bestTicket.ticket1.trainID) {
                                bestTicket = curTicket;
                            }
                            else if (curTicket.price == bestTicket.price && curTicket.during == bestTicket.during && curTicket.ticket1.trainID == bestTicket.ticket1.trainID && curTicket.ticket2.trainID < bestTicket.ticket2.trainID) {
                                bestTicket = curTicket;
                            }
                        }
                    }
                }
            }
            if (bestTicket.price == 1e9) {
                std::cout << 0 << '\n';
            }
            else {
                std::cout << bestTicket.ticket1 << '\n' << bestTicket.ticket2 << '\n';
            }
        }
        if (tokens.op_name == "buy_ticket") {
            sjtu::vector<User> vec0 = userBPT.findData(User{tokens._u});
            if (!vec0.empty() && vec0[0].login) {
                sjtu::vector vec1 = trainBPT.findData(Train{tokens._i});
                if (!vec1.empty() && vec1[0].release) {
                    sjtu::vector<std::string> mm_dd = split(tokens._d, '-');
                    MonthDate md{stringToInt(mm_dd[0]), stringToInt(mm_dd[1])};
                    int startIndex = -1, endIndex = -1;
                    HourMinute _setOffTime{}, _arriveTime{};
                    for (int i = 0; i < vec1[0].stationNum; i++) {
                        std::string stationN(vec1[0].stations[i]);
                        if (stationN == tokens._f) {
                            startIndex = i;
                            _setOffTime = vec1[0].setOffTime[i];
                        }
                        if (stationN == tokens._t) {
                            endIndex = i;
                            _arriveTime = vec1[0].arriveTime[i];
                            break;
                        }
                    }
                    if (startIndex >= endIndex || startIndex == -1 || endIndex == -1) {
                        std::cout << -1 << '\n';
                        continue;
                    }
                    Order newOrder{tokens._u, tokens._i, tokens._f, tokens._t, md, _setOffTime, _arriveTime};
                    newOrder.price = vec1[0].prices[endIndex] - vec1[0].prices[startIndex];
                    newOrder.seats = stringToInt(tokens._n);
                    newOrder.orderIndex = newOrderIndex++;
                    if (md - _setOffTime.dd - vec1[0].saleDate[0] < 0 || md - _setOffTime.dd > vec1[0].saleDate[1]) {
                        std::cout << -1 << '\n';
                        continue;
                    }
                    sjtu::vector<int> ticketI = ticketInfo.readTicketInfo(vec1[0].ticketInfoIndex + (md - _setOffTime.dd - vec1[0].saleDate[0]) * vec1[0].stationNum, vec1[0].stationNum);
                    int _maxSeat = -1;
                    for (int i = startIndex; i < endIndex; i++) {
                        _maxSeat = std::max(_maxSeat, ticketI[i]);
                    }
                    _maxSeat = vec1[0].seatNum - _maxSeat;
                    if (_maxSeat >= newOrder.seats) {
                        newOrder.orderStatus = OrderStatus::success;
                        for (int i = startIndex; i < endIndex; i++) {ticketI[i] += newOrder.seats;}
                        ticketInfo.writeTicketInfo(vec1[0].ticketInfoIndex + (md - _setOffTime.dd - vec1[0].saleDate[0]) * vec1[0].stationNum, ticketI);
                        orderBPT.addData(newOrder);
                        std::cout << newOrder.price * newOrder.seats << '\n';
                    }
                    else if (newOrder.seats <= vec1[0].seatNum && tokens._q == "true") {
                        newOrder.orderStatus = OrderStatus::pending;
                        PendingOrder newPendingOrder{tokens._u, tokens._i, tokens._f, tokens._t, md};
                        newPendingOrder.seats = newOrder.seats;
                        newPendingOrder.orderIndex = newOrder.orderIndex;
                        orderBPT.addData(newOrder);
                        pendingOrderBPT.addData(newPendingOrder);
                        std::cout << "queue\n";
                    }
                    else {
                        std::cout << -1 << '\n';
                    }
                }
                else {
                    std::cout << -1 << '\n';
                }
            }
            else {
                std::cout << -1 << '\n';
            }
        }
        if (tokens.op_name == "query_order") {
            sjtu::vector<User> vec0 = userBPT.findData(User{tokens._u});
            if (!vec0.empty() && vec0[0].login) {
                sjtu::vector<Order> orders = orderBPT.findData(Order{tokens._u});
                std::cout << orders.size() << '\n';
                for (const auto & order : orders) {std::cout << order << '\n';}
            }
            else {
                std::cout << -1 << '\n';
            }
        }
        if (tokens.op_name == "refund_ticket") {
            sjtu::vector<User> vec0 = userBPT.findData(User{tokens._u});
            int index;
            if (tokens._n.empty()) {
                index = 1;
            }
            else {
                index = stringToInt(tokens._n);
            }
            if (!vec0.empty() && vec0[0].login) {
                sjtu::vector<Order> orders = orderBPT.findData(Order{tokens._u});
                if (index <= orders.size()) {
                    Order curOrder = orders[index - 1];
                    curOrder.orderStatus = OrderStatus::refunded;
                    orderBPT.modifyData(orders[index - 1], curOrder);
                    if (orders[index - 1].orderStatus == OrderStatus::success) {
                        sjtu::vector vec1 = trainBPT.findData(Train{curOrder.trainID});
                        sjtu::map<std::string, int> stationIndex;
                        int startIndex = -1, endIndex = -1;
                        for (int i = 0; i < vec1[0].stationNum; i++) {
                            stationIndex[vec1[0].stations[i]] = i;
                        }
                        startIndex = stationIndex[curOrder.setOffStation];
                        endIndex = stationIndex[curOrder.arriveStation];
                        sjtu::vector<int> ticketI = ticketInfo.readTicketInfo(vec1[0].ticketInfoIndex + (curOrder.setOffDate - curOrder.setOffTime.dd - vec1[0].saleDate[0]) * vec1[0].stationNum, vec1[0].stationNum);
                        for (int i = startIndex; i < endIndex; i++) {ticketI[i] -= curOrder.seats;}
                        sjtu::vector<PendingOrder> pOrders = pendingOrderBPT.findData(PendingOrder{curOrder.trainID});
                        for (auto pOrder : pOrders) {
                            startIndex = stationIndex[pOrder.setOffStation];
                            endIndex = stationIndex[pOrder.arriveStation];
                            if (pOrder.setOffDate - vec1[0].setOffTime[startIndex].dd != curOrder.setOffDate - curOrder.setOffTime.dd) {continue;}
                            int _maxSeat = -1;
                            for (int j = startIndex; j < endIndex; j++) {_maxSeat = std::max(_maxSeat, ticketI[j]);}
                            if (pOrder.seats <= vec1[0].seatNum - _maxSeat) {
                                for (int j = startIndex; j < endIndex; j++) {ticketI[j] += pOrder.seats;}
                                sjtu::vector<Order> _orders = orderBPT.findData(Order{pOrder.username});
                                for (auto & _order : _orders) {
                                    if (_order.orderIndex == pOrder.orderIndex) {
                                        _order.orderStatus = OrderStatus::success;
                                        orderBPT.modifyData(_order, _order);
                                        break;
                                    }
                                }
                                pendingOrderBPT.removeData(pOrder);
                            }
                        }
                        ticketInfo.writeTicketInfo(vec1[0].ticketInfoIndex + (curOrder.setOffDate - curOrder.setOffTime.dd - vec1[0].saleDate[0]) * vec1[0].stationNum, ticketI);
                        std::cout << 0 << '\n';
                    }
                    else if (orders[index - 1].orderStatus == OrderStatus::pending) {
                        sjtu::vector<PendingOrder> pOrders = pendingOrderBPT.findData(PendingOrder{curOrder.trainID});
                        for (auto & pOrder : pOrders) {
                            if (pOrder.orderIndex == curOrder.orderIndex) {
                                pendingOrderBPT.removeData(pOrder);
                            }
                        }
                        std::cout << 0 << '\n';
                    }
                    else {
                        std::cout << -1 << '\n';
                    }
                }
                else {
                    std::cout << -1 << '\n';
                }
            }
            else {
                std::cout << -1 << '\n';
            }
        }
        if (tokens.op_name == "clean") {
            userBPT.clear();
            trainBPT.clear();
            stationBPT.clear();
            orderBPT.clear();
            pendingOrderBPT.clear();
            std::remove("system_file");
            std::remove("ticket_file");
            std::cout << 0 << '\n';
        }
    }
    flush();

    /*userBPT.clear();
    trainBPT.clear();
    stationBPT.clear();
    orderBPT.clear();
    pendingOrderBPT.clear();
    std::remove("system_file");
    std::remove("ticket_file");*/
}

sjtu::vector<Ticket> queryTicket(const std::string& _s, const std::string& _t, const std::string& _d) {
    sjtu::vector<std::string> mm_dd = split(_d, '-');
    MonthDate md{stringToInt(mm_dd[0]), stringToInt(mm_dd[1])};
    sjtu::vector<StationInfo> vec1 = stationBPT.findData(StationInfo{_s});
    sjtu::vector<StationInfo> vec2 = stationBPT.findData(StationInfo{_t});
    sjtu::vector<Ticket> vec;
    std::size_t pos1 = 0, pos2 = 0;
    while (pos1 < vec1.size() && pos2 < vec2.size()) {
        int val = strcmp(vec1[pos1].trainID, vec2[pos2].trainID);
        if (val == 0) {
            if (vec1[pos1].setOffTime < vec2[pos2].arriveTime) {
                std::string trainI{vec1[pos1].trainID, strlen(vec1[pos1].trainID)};
                sjtu::vector<Train> tmp = trainBPT.findData(Train{trainI});
                if (!tmp.empty()) {
                    if (tmp[0].saleDate[0] <= md - vec1[pos1].setOffTime.dd && tmp[0].saleDate[1] >= md - vec1[pos1].setOffTime.dd) {
                        int startIndex = 0, endIndex = 0;
                        for (int i = 0; i < tmp[0].stationNum; i++) {
                            std::string stationName{tmp[0].stations[i]};
                            if (stationName == _s) {startIndex = i;}
                            if (stationName == _t) {endIndex = i;}
                        }
                        if (startIndex >= endIndex) {goto End_;}
                        sjtu::vector<int> ticketI = ticketInfo.readTicketInfo(tmp[0].ticketInfoIndex + ((md - vec1[pos1].setOffTime.dd) - tmp[0].saleDate[0]) * tmp[0].stationNum, tmp[0].stationNum);
                        int _maxSeat = -1;
                        for (int i = startIndex; i < endIndex; i++) {_maxSeat = std::max(_maxSeat, ticketI[i]);}
                        Ticket _ticket{tmp[0].trainID, _s, _t, md, vec1[pos1].setOffTime, vec2[pos2].arriveTime, tmp[0].prices[endIndex] - tmp[0].prices[startIndex], tmp[0].seatNum - _maxSeat};
                        vec.push_back(_ticket);
                    }
                }
            }
            End_:;
            pos1++;
            pos2++;
        }
        if (val > 0) {pos2++;}
        if (val < 0) {pos1++;}
    }
    return vec;
}

sjtu::vector<Ticket> queryNextTicket(const std::string& _s, const std::string& _t, const std::string& _d, const Ticket& ticket1) {
    sjtu::vector<std::string> mm_dd = split(_d, '-');
    MonthDate md{stringToInt(mm_dd[0]), stringToInt(mm_dd[1])};
    md = md - ticket1.setOffTime.dd + ticket1.arriveTime.dd;
    sjtu::vector<StationInfo> vec1 = stationBPT.findData(StationInfo{_s});
    sjtu::vector<StationInfo> vec2 = stationBPT.findData(StationInfo{_t});
    sjtu::vector<Ticket> vec;
    std::size_t pos1 = 0, pos2 = 0;
    while (pos1 < vec1.size() && pos2 < vec2.size()) {
        int val = strcmp(vec1[pos1].trainID, vec2[pos2].trainID);
        if (val == 0) {
            std::string _trainI(vec1[pos1].trainID);
            if (_trainI == ticket1.trainID) {goto End;}
            if (vec1[pos1].setOffTime < vec2[pos2].arriveTime) {
                std::string trainI{vec1[pos1].trainID, strlen(vec1[pos1].trainID)};
                sjtu::vector<Train> tmp = trainBPT.findData(Train{trainI});
                if (!tmp.empty()) {
                    if (tmp[0].saleDate[1] >= md - vec1[pos1].setOffTime.dd) {
                        MonthDate _md = md;
                        if (tmp[0].saleDate[0] > md - vec1[pos1].setOffTime.dd) {
                            _md = tmp[0].saleDate[0] + vec1[pos1].setOffTime.dd;
                        }
                        else if (ticket1.arriveTime.hh > vec1[pos1].setOffTime.hh ||
                                ticket1.arriveTime.hh == vec1[pos1].setOffTime.hh &&
                                ticket1.arriveTime.mm > vec1[pos1].setOffTime.mm) {
                            _md = md + 1;
                            if (_md - vec1[pos1].setOffTime.dd > tmp[0].saleDate[1]) {goto End;}
                        }
                        int startIndex = 0, endIndex = 0;
                        for (int i = 0; i < tmp[0].stationNum; i++) {
                            std::string stationName{tmp[0].stations[i]};
                            if (stationName == _s) {startIndex = i;}
                            if (stationName == _t) {endIndex = i;}
                        }
                        if (startIndex >= endIndex) {goto End;}
                        sjtu::vector<int> ticketI = ticketInfo.readTicketInfo(tmp[0].ticketInfoIndex + ((_md - vec1[pos1].setOffTime.dd) - tmp[0].saleDate[0]) * tmp[0].stationNum, tmp[0].stationNum);
                        int _maxSeat = -1;
                        for (int i = startIndex; i < endIndex; i++) {_maxSeat = std::max(_maxSeat, ticketI[i]);}
                        Ticket _ticket{tmp[0].trainID, _s, _t, _md, vec1[pos1].setOffTime, vec2[pos2].arriveTime, tmp[0].prices[endIndex] - tmp[0].prices[startIndex], tmp[0].seatNum - _maxSeat};
                        vec.push_back(_ticket);
                    }
                }
            }
            End:;
            pos1++;
            pos2++;
        }
        if (val > 0) {pos2++;}
        if (val < 0) {pos1++;}
    }
    return vec;
}