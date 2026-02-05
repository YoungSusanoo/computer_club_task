#include <event/event.hpp>

std::strong_ordering club::operator<=>(const Event& a, const Event& b)
{
  auto get_time = [](const auto event)
  {
    return event.time;
  };

  Time a_time = std::visit(get_time, a);
  Time b_time = std::visit(get_time, b);
  return a_time <=> b_time;
}