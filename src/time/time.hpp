#ifndef CLUBTIME_HPP
#define CLUBTIME_HPP

#include <cstddef>

namespace club
{
  struct Time
  {
    const std::size_t hours;
    const std::size_t mins;

    Time() = delete;
    Time(std::size_t hours, std::size_t mins);
  };
}

#endif