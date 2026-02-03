#include <time/time.hpp>

#include <stdexcept>

club::Time::Time(std::size_t h, std::size_t m):
  hours(h),
  mins(m)
{
  if (h > 23 || m > 59)
  {
    throw std::invalid_argument("time/time.hpp: h > 23 and m > 59");
  }
}