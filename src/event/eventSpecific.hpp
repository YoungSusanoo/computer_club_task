#ifndef EVENTBASE_HPP
#define EVENTBASE_HPP

#include <variant>

namespace club
{
  struct ClientArrive
  {
    std::string client_name;
  };

  struct ClientSit
  {
    std::string client_name;
    std::size_t table;
  };

  struct ClientWait
  {
    std::string client_name;
  };

  struct ClientLeave
  {
    std::string client_name;
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
  };

  using EventSpecific = std::variant< ClientArrive, ClientSit, ClientWait, ClientLeave, EventError >;
};

#endif