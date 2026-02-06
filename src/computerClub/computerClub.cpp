#include <computerClub/computerClub.hpp>

club::ComputerClub::EventHandler::EventHandler(std::size_t t_num, std::size_t p, Time s, Time e):
  events {},
  clients {},
  tables(t_num),
  start { s },
  end { e },
  last_time { 0, 0 },
  queue {},
  free_tables { t_num },
  price { p }
{}

club::ComputerClub::ComputerClub(std::size_t tables_num, std::size_t price, Time start, Time end):
  handler_ { tables_num, price, start, end }
{}

void club::ComputerClub::handle_event(const Event& event)
{
  std::visit(handler_, event);
}

void club::ComputerClub::EventHandler::check_event_order(Time time)
{
  if (!events.empty() && time < last_time)
  {
    throw std::invalid_argument("computerClub.cpp: !events.empty() && time < last_time");
  }
}

void club::ComputerClub::EventHandler::count_payment(std::size_t table, Time time)
{
  if (!tables[table - 1].busy)
  {
    return;
  }

  std::size_t mins_elapsed = time.hours() * 60 + time.mins();
  mins_elapsed -= tables[table - 1].start_mins;
  tables[table - 1].total_mins += mins_elapsed;
  tables[table - 1].total_income += (mins_elapsed + 59) / 60 * price;
  tables[table - 1].busy = false;
  free_tables++;
}

void club::ComputerClub::EventHandler::seat_client(client_map_t::iterator it, std::size_t table, Time time)
{
  it->second = table;
  tables[table - 1].busy = true;
  tables[table - 1].start_mins = time.hours() * 60 + time.mins();
  free_tables--;
}

void club::ComputerClub::EventHandler::operator()(const ClientArrive& e)
{
  check_event_order(e.time);
  events.emplace_back(e);
  if (clients.contains(e.client_name))
  {
    events.emplace_back(EventError { e.time, EventError::Type::IDENTITY_VIOLATE });
    return;
  }
  if (e.time < start)
  {
    events.emplace_back(EventError { e.time, EventError::Type::NOT_OPENED });
    return;
  }
  clients[e.client_name] = {};
  last_time = e.time;
}

void club::ComputerClub::EventHandler::operator()(const ClientSit& e)
{
  check_event_order(e.time);
  if (e.table == 0 || e.table > tables.size())
  {
    throw std::invalid_argument("computerClub.cpp: e.table == 0 || e.table > tables.size()");
  }

  events.emplace_back(e);
  auto client_it = clients.find(e.client_name);
  if (client_it == clients.end())
  {
    events.emplace_back(EventError { e.time, EventError::Type::CLIENT_UNKNOWN });
    return;
  }
  if (client_it->second == e.table || tables[e.table - 1].busy)
  {
    events.emplace_back(EventError { e.time, EventError::Type::PLACE_IS_BUSY });
    return;
  }

  if (client_it->second.has_value())
  {
    count_payment(client_it->second.value(), e.time);
  }
  seat_client(client_it, e.table, e.time);
  last_time = e.time;
}

void club::ComputerClub::EventHandler::operator()(const ClientWait& e)
{
  check_event_order(e.time);
  events.emplace_back(e);
  if (free_tables)
  {
    events.emplace_back(EventError { e.time, EventError::Type::PLACES_AVAILABLE });
    return;
  }
  else if (queue.size() > tables.size())
  {
    events.emplace_back(ClientLeave { e.client_name, e.time, EventType::OUTPUT });
    return;
  }

  queue.emplace(e.client_name);
  last_time = e.time;
}

void club::ComputerClub::EventHandler::operator()(const ClientLeave& e)
{
  check_event_order(e.time);
  events.emplace_back(e);
  if (!clients.contains(e.client_name))
  {
    events.emplace_back(EventError { e.time, EventError::Type::CLIENT_UNKNOWN });
    return;
  }

  std::optional< std::size_t > table = clients[e.client_name];
  clients.erase(e.client_name);
  if (!table.has_value())
  {
    return;
  }

  count_payment(table.value(), e.time);
  if (!queue.empty())
  {
    seat_client(clients.find(queue.front()), table.value(), e.time);
    events.emplace_back(ClientSit { queue.front(), table.value(), e.time, EventType::OUTPUT });
    queue.pop();
  }
  last_time = e.time;
}

void club::ComputerClub::EventHandler::operator()(const EventError& e)
{
  throw std::invalid_argument("computerClub.cpp: operator()(const EventError& e) invoked");
}

void club::ComputerClub::complete_shift_internal()
{
  while (!handler_.clients.empty())
  {
    auto front_it = handler_.clients.begin();
    if (front_it->second.has_value())
    {
      handler_.count_payment(front_it->second.value(), handler_.end);
    }
    handler_.events.emplace_back(ClientLeave { front_it->first, handler_.end, EventType::OUTPUT });
    handler_.clients.erase(front_it);
  }
}
