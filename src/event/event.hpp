#ifndef CLUBEVENT_HPP
#define CLUBEVENT_HPP

#include <string>
#include <variant>

#include <time/time.hpp>

namespace club
{
  struct ClientArrive
  {
    std::string client_name;
    Time time;
  };

  struct ClientSit
  {
    std::string client_name;
    std::size_t table;
    Time time;
  };

  struct ClientWait
  {
    std::string client_name;
    Time time;
  };

  struct ClientLeave
  {
    std::string client_name;
    Time time;
  };

  struct EventError
  {
    enum class Type
    {
      CLIENT_UNKNOWN,
      PLACE_IS_BUSY,
      NOT_OPENED,
      IDENTITY_VIOLATE
    };

    Time time;
    Type type;
  };

  using Event = std::variant< ClientArrive, ClientSit, ClientWait, ClientLeave, EventError >;

  auto operator<=>(const Event& a, const Event& b);
}

#endif