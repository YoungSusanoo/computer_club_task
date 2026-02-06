#include <iostream>
#include <fstream>

#include <computerClub/computerClub.hpp>
#include <utils/utils.hpp>

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "No source provided\n";
    return 1;
  }

  std::ifstream in(argv[1]);
  if (!in)
  {
    std::cerr << "Can't read file\n";
    return 1;
  }

  std::size_t tables_n = 0;
  {
    std::string tables_line;
    std::getline(in, tables_line);
    try
    {
      std::size_t pos = 0;
      tables_n = std::stoull(tables_line, &pos);
      if (pos != tables_line.size() || tables_n == 0)
      {
        std::cerr << "Invalid line: " << tables_line << '\n';
        return 1;
      }
    }
    catch (...)
    {
      std::cerr << "Invalid line: " << tables_line << '\n';
      return 1;
    }
  }

  club::Time start;
  club::Time end;
  {
    std::string time_line;
    std::getline(in, time_line);
    try
    {
      std::string_view view = time_line;
      start = club::parse_time(view.substr(0, 5));
      end = club::parse_time(view.substr(6, 5));
      if (start > end)
      {
        std::cerr << "Invalid line: " << time_line << '\n';
        return 1;
      }
    }
    catch (...)
    {
      std::cerr << "Invalid line: " << time_line << '\n';
      return 1;
    }
  }

  std::size_t price = 0;
  {
    std::string price_line;
    std::getline(in, price_line);
    try
    {
      std::size_t pos = 0;
      price = std::stoull(price_line, &pos);
      if (pos != price_line.size() || price == 0)
      {
        std::cerr << "Invalid line: " << price_line << '\n';
        return 1;
      }
    }
    catch (...)
    {
      std::cerr << "Invalid line: " << price_line << '\n';
      return 1;
    }
  }

  club::ComputerClub pc_club(tables_n, price, start, end);
  while (in)
  {
    std::string event_line;
    std::getline(in, event_line);
    club::Event event;
    try
    {
      if (!event_line.empty())
      {
        pc_club.handle_event(club::event_from_string(event_line));
      }
    }
    catch (...)
    {
      std::cerr << "Invalid line: " << event_line << '\n';
      return 1;
    }
  }

  auto tables = pc_club.complete_shift();
  std::cout << club::time_to_string(start) << '\n';
  for (const auto& event : pc_club.get_events())
  {
    std::cout << club::event_to_string(event) << '\n';
  }
  std::cout << club::time_to_string(end) << '\n';
  for (std::size_t i = 0; i < tables_n; i++)
  {
    std::cout << i + 1 << ' ' << tables[i].total_income << ' ';
    std::size_t curr_hours = tables[i].total_mins / 60;
    std::size_t curr_mins = tables[i].total_mins % 60;
    std::cout << club::time_to_string(club::Time { curr_hours, curr_mins }) << '\n';
  }
  return 0;
}
