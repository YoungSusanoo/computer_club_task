#ifndef CLUBIOUTILS_HPP
#define CLUBIOUTILS_HPP

#include <string>

#include <event/event.hpp>

namespace club
{
  Event event_from_string(std::string_view str);
  Time parse_time(std::string_view str);

  std::string time_to_string(Time time);
  std::string event_to_string(const Event& event);
}

#endif