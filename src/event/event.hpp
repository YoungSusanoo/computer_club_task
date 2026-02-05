#ifndef CLUBEVENT_HPP
#define CLUBEVENT_HPP

#include <string>
#include <variant>

#include <time/time.hpp>

namespace club
{
  enum class EventType
  {
    INPUT,
    OUTPUT
  };

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
    EventType type;
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
    EventType type;
  };

  struct EventError
  {
    Time time;
    enum class Type
    {
      CLIENT_UNKNOWN,
      PLACE_IS_BUSY,
      NOT_OPENED,
      IDENTITY_VIOLATE
    } error;
  };

  using Event = std::variant< ClientArrive, ClientSit, ClientWait, ClientLeave, EventError >;

  std::strong_ordering operator<=>(const Event& a, const Event& b);
}

#endif