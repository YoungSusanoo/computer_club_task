#ifndef COMPUTERCLUB_HPP
#define COMPUTERCLUB_HPP

#include <ranges>
#include <map>
#include <vector>
#include <queue>
#include <optional>

#include <event/event.hpp>

namespace club
{
  class ComputerClub
  {
  public:
    struct Table
    {
      bool busy;
      std::size_t start_mins;
      std::size_t total_mins;
      std::size_t total_income;
    };

    ComputerClub(std::size_t tables_num, std::size_t price, Time start, Time end);

    void handle_event(const Event& event);

    auto get_events()
    {
      return handler_.events | std::ranges::views::all;
    }
    auto complete_shift()
    {
      complete_shift_internal();
      return handler_.tables | std::ranges::views::all;
    }

  private:
    struct EventHandler
    {
      using client_map_t = std::map< std::string, std::optional< std::size_t > >;
      std::vector< Event > events;
      client_map_t clients;
      std::vector< Table > tables;
      Time start;
      Time end;
      Time last_time;
      std::queue< std::string > queue;
      std::size_t free_tables;
      std::size_t price;

      EventHandler(std::size_t tables_num, std::size_t price, Time start, Time end);

      void check_event_order(Time time);
      void count_payment(std::size_t table, Time time);
      void seat_client(client_map_t::iterator it, std::size_t table, Time time);

      void operator()(const ClientArrive& e);
      void operator()(const ClientSit& e);
      void operator()(const ClientWait& e);
      void operator()(const ClientLeave& e);
      void operator()(const EventError& e);
    } handler_;

    void complete_shift_internal();
  };
}

#endif