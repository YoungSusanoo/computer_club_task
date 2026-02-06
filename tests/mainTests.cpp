#include <gtest/gtest.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#define main club_main
#include "main.cpp"
#undef main

namespace
{
  std::filesystem::path write_temp_file(const std::string& contents)
  {
    auto base = std::filesystem::temp_directory_path();
    auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
    std::filesystem::path path = base / ("club_test_" + std::to_string(stamp) + ".txt");
    std::ofstream out(path);
    out << contents;
    out.close();
    return path;
  }

  int run_main_with_args(const std::vector< std::string >& args)
  {
    std::vector< char* > argv;
    argv.reserve(args.size());
    for (const auto& arg : args)
    {
      argv.push_back(const_cast< char* >(arg.c_str()));
    }
    return club_main(static_cast< int >(argv.size()), argv.data());
  }
}

TEST(MainTest, RequiresSourceFile)
{
  testing::internal::CaptureStderr();
  int result = run_main_with_args({ "computer_club_task" });
  std::string stderr_output = testing::internal::GetCapturedStderr();

  EXPECT_EQ(result, 1);
  EXPECT_EQ(stderr_output, "No source provided\n");
}

TEST(MainTest, ReportsMissingFile)
{
  testing::internal::CaptureStderr();
  int result = run_main_with_args({ "computer_club_task", "missing_file.txt" });
  std::string stderr_output = testing::internal::GetCapturedStderr();

  EXPECT_EQ(result, 1);
  EXPECT_EQ(stderr_output, "Can't read file\n");
}

TEST(MainTest, RejectsInvalidHeaderLine)
{
  auto path = write_temp_file("0\n09:00 10:00\n10\n");
  testing::internal::CaptureStderr();
  int result = run_main_with_args({ "computer_club_task", path.string() });
  std::string stderr_output = testing::internal::GetCapturedStderr();

  EXPECT_EQ(result, 1);
  EXPECT_EQ(stderr_output, "Invalid line: 0\n");
}

TEST(MainTest, OutputsSummaryForEmptyDay)
{
  auto path = write_temp_file("1\n09:00 10:00\n10\n");
  testing::internal::CaptureStdout();
  int result = run_main_with_args({ "computer_club_task", path.string() });
  std::string stdout_output = testing::internal::GetCapturedStdout();

  EXPECT_EQ(result, 0);
  EXPECT_EQ(stdout_output, "09:00\n10:00\n1 0 00:00\n");
}
