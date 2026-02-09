#include <utils/utils.hpp>

#include <stdexcept>
#include <charconv>

namespace club
{
  constexpr std::size_t min_event_str_length = 9;
  constexpr std::size_t time_start = 0;
  constexpr std::size_t time_delim = 2;
  constexpr std::size_t time_length = 5;
  constexpr std::size_t type_index = 6;
  constexpr std::size_t client_name_start = 8;

  constexpr char client_arrive[] = "1";
  constexpr char client_wait[] = "3";
  constexpr char error_event[] = "13";

  constexpr char client_sit_in[] = "2";
  constexpr char client_leave_in[] = "4";

  constexpr char client_leave_out[] = "11";
  constexpr char client_sit_out[] = "12";

  constexpr char client_unknown_er[] = "ClientUnknown";
  constexpr char place_is_busy_er[] = "PlaceIsBusy";
  constexpr char not_opened_er[] = "NotOpenYet";
  constexpr char places_available_er[] = "ICanWaitNoLonger!";
  constexpr char identity_violate_er[] = "YouShallNotPass";

  struct EventStringMaker
  {
    std::string operator()(const ClientArrive& e);
    std::string operator()(const ClientSit& e);
    std::string operator()(const ClientWait& e);
    std::string operator()(const ClientLeave& e);
    std::string operator()(const EventError& e);
  };
}

club::Event club::event_from_string(std::string_view str)
{
  if (str.length() < min_event_str_length)
  {
    throw std::invalid_argument("utils.cpp: str.length() < min_event_str_length");
  }

  Time time = parse_time(str.substr(time_start, time_length));

  if (str[type_index - 1] != ' ' || str[type_index + 1] != ' ')
  {
    throw std::invalid_argument("utils.cpp: str[type_index - 1] != ' ' || str[type_index + 1] != ' '");
  }
  if (str[type_index] < '1' || str[type_index] > '4')
  {
    throw std::invalid_argument("utils.cpp: str[type_index] < '1' || str[type_index] > '4'");
  }

  std::size_t client_name_end = str.find(' ', client_name_start);
  if (client_name_end == client_name_start)
  {
    throw std::invalid_argument("utils.cpp: client_name_end == client_name_start");
  }
  std::string client { str.substr(client_name_start, client_name_end - client_name_start) };

  if (str[type_index] == '2')
  {
    if (client_name_end == std::string_view::npos)
    {
      throw std::invalid_argument("utils.cpp: str[type_index] == '2' && client_name_end == std::string_view::npos");
    }

    std::size_t space_after_client = str.find(' ', client_name_end + 1);
    if (space_after_client != std::string_view::npos)
    {
      throw std::invalid_argument("utils.cpp: space_after_client != std::string_view::npos");
    }
    std::string_view table_num_str = str.substr(client_name_end + 1, space_after_client - client_name_end);

    std::size_t table_num = 0;
    auto [ptr, table_n_ec] = std::from_chars(table_num_str.data(), table_num_str.data() + table_num_str.size(), table_num);
    if (table_n_ec != std::errc())
    {
      throw std::invalid_argument("utils.cpp: table_n_ec != std::errc()");
    }
    if (ptr != table_num_str.data() + table_num_str.size())
    {
      throw std::invalid_argument("utils.cpp: ptr != table_num_str.data() + table_num_str.size()");
    }
    if (table_num < 1)
    {
      throw std::invalid_argument("utils.cpp: table_num < 1");
    }

    return Event { std::in_place_type< ClientSit >, client, table_num, time, EventType::INPUT };
  }
  else if (client_name_end != std::string_view::npos)
  {
    throw std::invalid_argument("utils.cpp: str[type_index] != '2' && client_name_end != std::string_view::npos");
  }

  if (str[type_index] == '1')
  {
    return Event { std::in_place_type< ClientArrive >, client, time };
  }
  else if (str[type_index] == '3')
  {
    return Event { std::in_place_type< ClientWait >, client, time };
  }
  else
  {
    return Event { std::in_place_type< ClientLeave >, client, time, EventType::INPUT };
  }
}

club::Time club::parse_time(std::string_view str)
{
  std::size_t hours = 0;
  auto [parsed_hours_ptr, hours_ec] = std::from_chars(str.data(), str.data() + 2, hours);
  if (parsed_hours_ptr != str.data() + 2)
  {
    throw std::invalid_argument("utils.cpp: parsed_hours_ptr != str.data() + 2");
  }
  if (hours > 23)
  {
    throw std::invalid_argument("utils.cpp: hours > 23");
  }

  if (str[time_delim] != ':')
  {
    throw std::invalid_argument("utils.cpp: str[time_delim] != ':'");
  }

  std::size_t mins = 0;
  auto [parsed_mins_ptr, mins_ec] = std::from_chars(str.data() + time_delim + 1, str.data() + time_delim + 3, mins);
  if (parsed_mins_ptr != str.data() + time_delim + 3)
  {
    throw std::invalid_argument("utils.cpp: parsed_mins_ptr != str.data() + time_delim + 3");
  }
  if (mins > 59)
  {
    throw std::invalid_argument("utils.cpp: mins > 59");
  }

  return { hours, mins };
}

std::string club::event_to_string(const Event& event)
{
  return std::visit(EventStringMaker {}, event);
}

std::string club::time_to_string(Time time)
{
  std::string result;
  result += std::to_string(time.hours() / 10);
  result += std::to_string(time.hours() % 10);
  result += ":";
  result += std::to_string(time.mins() / 10);
  result += std::to_string(time.mins() % 10);
  return result;
}

std::string club::EventStringMaker::operator()(const ClientArrive& e)
{
  return time_to_string(e.time) + " " + client_arrive + " " + e.client_name;
}

std::string club::EventStringMaker::operator()(const ClientSit& e)
{
  std::string result;
  result += time_to_string(e.time) + " ";
  if (e.type == EventType::INPUT)
  {
    result += client_sit_in;
  }
  else
  {
    result += client_sit_out;
  }
  result += " ";
  result += e.client_name + " " + std::to_string(e.table);
  return result;
}

std::string club::EventStringMaker::operator()(const ClientWait& e)
{
  return time_to_string(e.time) + " " + client_wait + " " + e.client_name;
}

std::string club::EventStringMaker::operator()(const ClientLeave& e)
{
  std::string result;
  result += time_to_string(e.time) + " ";
  if (e.type == EventType::INPUT)
  {
    result += client_leave_in;
  }
  else
  {
    result += client_leave_out;
  }
  result += " ";
  result += e.client_name;
  return result;
}

std::string club::EventStringMaker::operator()(const EventError& e)
{
  std::string result;
  result += time_to_string(e.time) + " ";
  result += error_event;
  result += " ";
  if (e.error == EventError::Type::CLIENT_UNKNOWN)
  {
    result += client_unknown_er;
  }
  else if (e.error == EventError::Type::PLACE_IS_BUSY)
  {
    result += place_is_busy_er;
  }
  else if (e.error == EventError::Type::NOT_OPENED)
  {
    result += not_opened_er;
  }
  else if (e.error == EventError::Type::PLACES_AVAILABLE)
  {
    result += places_available_er;
  }
  else
  {
    result += identity_violate_er;
  }
  return result;
}