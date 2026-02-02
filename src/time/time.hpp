#ifndef CLUBTIME_HPP
#define CLUBTIME_HPP

namespace club
{
  struct Time
  {
    const hours;
    const mins;

    Time() = delete;
    Time(std::size_t hours, std::size_t mins);
  };
}

#endif