#ifndef TICKETINFO_H
#define TICKETINFO_H
#include <fstream>
#include "../STLite/vector/vector.hpp"
struct TicketInfo {
    std::fstream ticketFile;

    TicketInfo() {
        ticketFile.open("ticket_file", std::ios::in|std::ios::out);
        if (!ticketFile.is_open()) {
            ticketFile.open("ticket_file", std::ios::out);
            ticketFile.close();
            ticketFile.open("ticket_file", std::ios::in|std::ios::out);
        }
    }

    ~TicketInfo() {
        ticketFile.close();
    }

    void writeTicketInfo(int index, const sjtu::vector<int>& vec) {
        ticketFile.seekp(index * sizeof(int));
        int len = static_cast<int>(vec.size());
        auto* ticketNum = new int[len]{};
        for (int i = 0; i < len; i++) {
            ticketNum[i] = vec[i];
        }
        ticketFile.write(reinterpret_cast<char*> (ticketNum), len * sizeof(int));
        delete[] ticketNum;
    }

    sjtu::vector<int> readTicketInfo(int index, int len) {
        sjtu::vector<int> ret;
        int* ticketNum = new int[len]{};
        ticketFile.seekp(index * sizeof(int));
        ticketFile.read(reinterpret_cast<char*>(ticketNum), len * sizeof(int));
        for (int i = 0; i < len; i++) {
            ret.push_back(ticketNum[i]);
        }
        delete[] ticketNum;
        return ret;
    }
};
#endif //TICKETINFO_H
