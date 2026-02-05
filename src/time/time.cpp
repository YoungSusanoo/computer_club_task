#include <time/time.hpp>

#include <stdexcept>

club::Time::Time(std::size_t h, std::size_t m):
  hours_(h),
  mins_(m)
{
  if (h > 23 || m > 59)
  {
    throw std::invalid_argument("time/time.hpp: h > 23 and m > 59");
  }
}

std::size_t club::Time::hours() const noexcept
{
  return hours_;
}

std::size_t club::Time::mins() const noexcept
{
  return mins_;
}

std::strong_ordering club::operator<=>(Time a, Time b)
{
  return (a.hours() * 60 + a.mins()) <=> (b.hours() * 60 + b.mins());
}