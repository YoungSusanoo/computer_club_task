#ifndef EVENTBASE_HPP
#define EVENTBASE_HPP

#include <variant>

namespace club
{
  struct ClientArrive
  {
    std::string client_name;
    std::size_t hours;
    std::size_t mins;
  };

  struct ClientSit
  {
    std::string client_name;
    std::size_t hours;
    std::size_t mins;
    std::size_t table;
  };

  struct ClientWait
  {
    std::string client_name;
    std::size_t hours;
    std::size_t mins;
  };

  struct ClientLeave
  {
    std::string client_name;
    std::size_t hours;
    std::size_t mins;
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
    std::size_t hours;
    std::size_t mins;
  };

  using EventBase = std::variant< ClientArrive, ClientSit, ClientWait, ClientLeave, EventError >;
};

#endif