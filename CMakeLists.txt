cmake_minimum_required(VERSION 3.16)

project(computer_club_task)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS 
    src/event/event.hpp
    src/event/eventSpecific.hpp
    src/time/time.hpp
)
set(SOURCES 
    src/main.cpp
    src/time/time.cpp
)

add_executable(computer_club_task ${SOURCES})
target_include_directories(computer_club_task PUBLIC src)