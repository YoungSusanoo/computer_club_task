cmake_minimum_required(VERSION 3.16)

project(computer_club_task)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS 
    src/utils/utils.hpp
    src/event/event.hpp
    src/time/time.hpp
    src/computerClub/computerClub.hpp
)
set(SOURCES 
    src/utils/utils.cpp
    src/main.cpp
    src/event/event.cpp
    src/time/time.cpp
    src/computerClub/computerClub.cpp
)

add_executable(computer_club_task ${SOURCES})
target_include_directories(computer_club_task PUBLIC src)

include(CTest)

option(BUILD_TESTING "Build tests" OFF)
if (BUILD_TESTING)
  include(FetchContent)

  find_package(GTest QUIET)
  if (NOT GTest_FOUND)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
  endif()

  add_executable(computer_club_task_tests
    tests/computerClubTests.cpp
    tests/mainTests.cpp
    src/utils/utils.cpp
    src/event/event.cpp
    src/time/time.cpp
    src/computerClub/computerClub.cpp
  )
  target_include_directories(computer_club_task_tests PUBLIC src)
  target_link_libraries(computer_club_task_tests PRIVATE GTest::gtest_main)

  include(GoogleTest)
  gtest_discover_tests(computer_club_task_tests)
endif()
