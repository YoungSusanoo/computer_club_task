#ifndef CLUBEVENT_HPP
#define CLUBEVENT_HPP

#include <string>

#include <event/eventSpecific.hpp>
#include <time/time.hpp>

namespace club
{
  class Event
  {
  public:
    Event(Time time, EventSpecific specific);

    const EventSpecific& get_specific();
    Time get_time();

  private:
    EventSpecific base_;
    Time time_;
  };

  auto operator<=>(const Event& a, const Event& b);
}

#endif