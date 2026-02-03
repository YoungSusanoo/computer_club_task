cmake_minimum_required(VERSION 3.16)

project(computer_club_task)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS 
    src/event/event.hpp
    src/time/time.hpp
)
set(SOURCES 
    src/main.cpp
    src/event/event.cpp
    src/time/time.cpp
)

add_executable(computer_club_task ${SOURCES})
target_include_directories(computer_club_task PUBLIC src)