#include <gtest/gtest.h>

#include <algorithm>

#include <computerClub/computerClub.hpp>
#include <event/event.hpp>
#include <utils/utils.hpp>

namespace
{
  std::vector< club::Event > collect_events(club::ComputerClub& club)
  {
    std::vector< club::Event > events;
    for (const auto& event : club.get_events())
    {
      events.push_back(event);
    }
    return events;
  }

  std::vector< std::string > collect_event_strings(club::ComputerClub& club)
  {
    std::vector< std::string > events;
    for (const auto& event : club.get_events())
    {
      events.push_back(club::event_to_string(event));
    }
    return events;
  }
}

TEST(TimeTest, ConstructsAndCompares)
{
  club::Time start { 9, 15 };
  club::Time end { 10, 0 };

  EXPECT_EQ(start.hours(), 9u);
  EXPECT_EQ(start.mins(), 15u);
  EXPECT_LT(start, end);
}

TEST(TimeTest, RejectsInvalid)
{
  EXPECT_THROW((club::Time { 24, 0 }), std::invalid_argument);
  EXPECT_THROW((club::Time { 12, 60 }), std::invalid_argument);
}

TEST(EventTest, OrdersByTime)
{
  club::Event first = club::ClientArrive {
    "alice", club::Time { 9, 0 }
  };
  club::Event second = club::ClientArrive {
    "bob", club::Time { 10, 0 }
  };

  EXPECT_LT(first, second);
}

TEST(UtilsTest, ParsesAndFormatsEvents)
{
  auto arrive = club::event_from_string("09:00 1 client");
  auto sit = club::event_from_string("09:05 2 client 1");
  auto wait = club::event_from_string("09:06 3 client");
  auto leave = club::event_from_string("09:07 4 client");

  EXPECT_EQ(club::event_to_string(arrive), "09:00 1 client");
  EXPECT_EQ(club::event_to_string(sit), "09:05 2 client 1");
  EXPECT_EQ(club::event_to_string(wait), "09:06 3 client");
  EXPECT_EQ(club::event_to_string(leave), "09:07 4 client");
}

TEST(UtilsTest, RejectsBadInput)
{
  EXPECT_THROW(club::event_from_string("09:00 5 client"), std::invalid_argument);
  EXPECT_THROW(club::event_from_string("09:00 2 client"), std::invalid_argument);
  EXPECT_THROW(club::event_from_string("09:00 1"), std::invalid_argument);
}

TEST(UtilsTest, FormatsErrors)
{
  club::Event error = club::Event {
    std::in_place_type< club::EventError >, club::Time { 9, 1 },
        club::EventError::Type::PLACES_AVAILABLE
  };

  EXPECT_EQ(club::event_to_string(error), "09:01 13 ICanWaitNoLonger");
}

TEST(ComputerClubTest, EmitsErrorWhenClosed)
{
  club::ComputerClub club(1, 10, club::Time { 9, 0 }, club::Time { 10, 0 });

  club.handle_event(club::event_from_string("08:59 1 alice"));

  auto events = collect_event_strings(club);
  ASSERT_EQ(events.size(), 2u);
  EXPECT_EQ(events[0], "08:59 1 alice");
  EXPECT_EQ(events[1], "08:59 13 NotOpenYet");
}

TEST(ComputerClubTest, SeatsClientAndCharges)
{
  club::ComputerClub club(1, 10, club::Time { 9, 0 }, club::Time { 12, 0 });

  club.handle_event(club::event_from_string("09:00 1 alice"));
  club.handle_event(club::event_from_string("09:10 2 alice 1"));
  club.handle_event(club::event_from_string("10:05 4 alice"));

  auto tables = club.complete_shift();
  ASSERT_EQ(tables.size(), 1u);
  EXPECT_EQ(tables[0].total_income, 20u);
  EXPECT_EQ(tables[0].total_mins, 55u);
}

TEST(ComputerClubTest, QueueEmitsSeatOnLeave)
{
  club::ComputerClub club(1, 10, club::Time { 9, 0 }, club::Time { 12, 0 });

  club.handle_event(club::event_from_string("09:00 1 alice"));
  club.handle_event(club::event_from_string("09:01 2 alice 1"));
  club.handle_event(club::event_from_string("09:02 1 bob"));
  club.handle_event(club::event_from_string("09:03 3 bob"));
  club.handle_event(club::event_from_string("09:30 4 alice"));

  auto events = collect_event_strings(club);
  auto it = std::find(events.begin(), events.end(), "09:30 12 bob 1");
  EXPECT_NE(it, events.end());
}

TEST(ComputerClubTest, CompleteShiftAddsLeaveEvents)
{
  club::ComputerClub club(2, 5, club::Time { 9, 0 }, club::Time { 11, 0 });

  club.handle_event(club::event_from_string("09:00 1 alice"));
  club.handle_event(club::event_from_string("09:05 2 alice 1"));
  club.handle_event(club::event_from_string("09:10 1 bob"));
  club.handle_event(club::event_from_string("09:12 2 bob 2"));

  auto tables = club.complete_shift();
  auto events = collect_event_strings(club);

  EXPECT_EQ(tables[0].total_income, 10u);
  EXPECT_EQ(tables[1].total_income, 10u);

  EXPECT_NE(std::find(events.begin(), events.end(), "11:00 11 alice"), events.end());
  EXPECT_NE(std::find(events.begin(), events.end(), "11:00 11 bob"), events.end());
}
