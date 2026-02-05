#include <computerClub/computerClub.hpp>

club::ComputerClub::EventHandler::EventHandler(std::size_t t_num, std::size_t p, Time s, Time e):
  events {},
  clients {},
  tables(t_num),
  start { s },
  end { e },
  queue {},
  price { p }
{}

club::ComputerClub::ComputerClub(std::size_t tables_num, std::size_t price, Time start, Time end):
  handler_ { tables_num, price, start, end }
{}

void club::ComputerClub::handle_event(const Event& event)
{
  std::visit(handler_, event);
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
  events.emplace(Event { e });
}

void club::ComputerClub::EventHandler::operator()(const ClientSit& e)
{
  auto client_it = clients.find(e.client_name);
  if (client_it == clients.end())
  {
    events.emplace(EventError { e.time, EventError::Type::CLIENT_UNKNOWN });
    return;
  }
  if (*client_it == e.table || tables[e.table - 1].busy)
  {
    events.emplace(EventError { e.time, EventError::Type::PLACE_IS_BUSY });
    return;
  }

  count_payment(*client_it, e.time);
  *client_it = e.table;
  tables[e.table - 1].busy = true;
  tables[e.table - 1].start_mins = e.time.hours() * 60 + e.time.mins();
  queue++;
  events.emplace(Event { e });
}

void club::ComputerClub::EventHandler::operator()(const ClientWait& e)
{
  if (queue < tables_n)
  {}
}
