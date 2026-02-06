#include <computerClub/computerClub.hpp>

club::ComputerClub::EventHandler::EventHandler(std::size_t t_num, std::size_t p, Time s, Time e):
  events {},
  clients {},
  tables(t_num),
  start { s },
  end { e },
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

void club::ComputerClub::EventHandler::count_payment(std::size_t table, Time time)
{
  if (!table || !tables[table - 1].busy)
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

void club::ComputerClub::EventHandler::operator()(const ClientArrive& e)
{
  if (clients.contains(e.client_name))
  {
    events.emplace(EventError { e.time, EventError::Type::IDENTITY_VIOLATE });
    return;
  }
  if (e.time < start)
  {
    events.emplace(EventError { e.time, EventError::Type::NOT_OPENED });
    return;
  }
  clients[e.client_name] = 0;
  events.emplace(e);
}

void club::ComputerClub::EventHandler::operator()(const ClientSit& e)
{
  auto client_it = clients.find(e.client_name);
  if (client_it == clients.end())
  {
    events.emplace(EventError { e.time, EventError::Type::CLIENT_UNKNOWN });
    return;
  }
  if (client_it->second == e.table || tables[e.table - 1].busy)
  {
    events.emplace(EventError { e.time, EventError::Type::PLACE_IS_BUSY });
    return;
  }

  count_payment(client_it->second, e.time);
  client_it->second = e.table;
  tables[e.table - 1].busy = true;
  tables[e.table - 1].start_mins = e.time.hours() * 60 + e.time.mins();
  free_tables--;
  events.emplace(e);
}

void club::ComputerClub::EventHandler::operator()(const ClientWait& e)
{
  if (free_tables)
  {
    events.emplace(EventError { e.time, EventError::Type::PLACES_AVAILABLE });
    return;
  }
  else if (queue.size() > tables.size())
  {
    events.emplace(ClientLeave { e.client_name, e.time, EventType::OUTPUT });
    return;
  }

  queue.emplace(e.client_name);
  events.emplace(e);
}

void club::ComputerClub::EventHandler::operator()(const ClientLeave& e)
{
  if (!clients.contains(e.client_name))
  {
    events.emplace(EventError { e.time, EventError::Type::CLIENT_UNKNOWN });
    return;
  }

  std::size_t table = clients[e.client_name];
  clients.erase(e.client_name);
  events.emplace(e);
  if (table == 0)
  {
    return;
  }

  if (!queue.empty())
  {
    clients[queue.front()] = table;
    operator()(ClientSit { queue.front(), table, e.time, EventType::OUTPUT });
    queue.pop();
  }
  else
  {
    count_payment(table, e.time);
  }
}

void club::ComputerClub::EventHandler::operator()(const EventError& e)
{
  return;
}

void club::ComputerClub::complete_shift_internal()
{
  while (!handler_.clients.empty())
  {
    auto front_it = handler_.clients.begin();
    handler_.count_payment(front_it->second, handler_.end);
    handler_.events.emplace(ClientLeave { front_it->first, handler_.end, EventType::OUTPUT });
    handler_.clients.erase(front_it);
  }
}
