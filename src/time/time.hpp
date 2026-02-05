#ifndef CLUBTIME_HPP
#define CLUBTIME_HPP

#include <cstddef>
#include <compare>

namespace club
{
  struct Time
  {
    Time() = default;
    Time(std::size_t hours, std::size_t mins);

    std::size_t hours() const noexcept;
    std::size_t mins() const noexcept;

  private:
    std::size_t hours_;
    std::size_t mins_;
  };

  std::strong_ordering operator<=>(Time a, Time b);
}

#endif