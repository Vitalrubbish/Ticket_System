cmake_minimum_required(VERSION 3.22)
project(code)

set(CMAKE_CXX_STANDARD 20)

add_executable(code
        main.cpp
        database/BPT.h
        database/BPT.cpp
        database/CacheManager.h
        database/HashTable.h

        include/tokenscanner.h
        include/HourMinute.h
        include/MonthDate.h
        include/tools.h
        include/User.h
        include/Train.h
        include/TicketInfo.h
        include/StationInfo.h
        include/Tickets.h
        include/Order.h
        include/stationHash.h

        src/tokenscanner.cpp
        src/HourMinute.cpp
        src/MonthDate.cpp
        src/User.cpp
        src/Train.cpp
        src/StationInfo.cpp
        src/Order.cpp

        STLite/vector/exceptions.hpp
        STLite/vector/utility.hpp
        STLite/vector/vector.hpp
        STLite/map/exceptions.hpp
        STLite/map/utility.hpp
        STLite/map/map.hpp
        STLite/list/list.hpp
)
