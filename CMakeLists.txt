cmake_minimum_required(VERSION 3.22)
project(code)

set(CMAKE_CXX_STANDARD 20)

add_executable(code
        main.cpp
        database/BPT.h
        database/BPT.cpp

        include/tokenscanner.h
        include/HourMinute.h
        include/MonthDate.h
        include/tools.h

        src/tokenscanner.cpp
        src/HourMinute.cpp
        src/MonthDate.cpp
        src/User.cpp

        STLite/exceptions.hpp
        STLite/utility.hpp
        STLite/vector.hpp
)
