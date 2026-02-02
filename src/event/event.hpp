#ifndef CLUBEVENT_HPP
#define CLUBEVENT_HPP

#include <string>

#include "eventBase.hpp"

namespace club
{
  class Event
  {
  public:
    Event(std::string_view source);

    const EventBase& get_base();

  private:
    EventBase base_;
  }
}

#endif