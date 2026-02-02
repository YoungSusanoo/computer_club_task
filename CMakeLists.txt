cmake_minimum_required(VERSION 3.16)

project(computer_club_task)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES src/main.cpp)

add_executable(computer_club_task ${SOURCES})
