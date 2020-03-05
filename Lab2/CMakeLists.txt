cmake_minimum_required(VERSION 3.15)
project(Lab2)

set(CMAKE_CXX_STANDARD 17)

include_directories(Lab2)

add_executable(Creator
        Creator/Main.cpp)
add_executable(Reporter
        Reporter/Main.cpp)
add_executable(MainApp
        Lab2/Employee.cpp
        Lab2/Employee.h
        Lab2/Main.cpp
        Lab2/Subprocess.cpp
        Lab2/Subprocess.h)
