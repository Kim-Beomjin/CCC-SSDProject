#ifdef _DEBUG
#include "gmock/gmock.h"
#include "hostInterface.h"
#include <string>
#include <vector>
#include <array>

class SSD_IT_Fixture : public testing::Test
{
public:
  void SetUp()
  {
    host->SetSSD(new BufferedSSD());
  }

protected:
  HostInterface* host = HostInterface::GetInstance();

  static const int VALID_READ_ARGC = 3;
  static const int VALID_WRITE_ARGC = 4;
  static const int VALID_ERASE_ARGC = 4;
  static const int INVALID_READ_ARGC = 5;
  static const int INVALID_WRITE_ARGC = 3;
  static const int INVALID_ERASE_ARGC = 3;
  const std::vector<std::array<char*, 3>> VALID_READ_ARGV_LIST = {
    {(char*)"SSD.exe", (char*)"R", (char*)"59"},
    {(char*)"SSD.exe", (char*)"R", (char*)"99"},
    {(char*)"SSD.exe", (char*)"R", (char*)"12"},
  };

  const std::vector<std::array<char*, 3>> INVALID_READ_ARGV_LIST = {
    {(char*)"SSD.exe", (char*)"R", (char*)"-1"},
    {(char*)"SSD.exe", (char*)"R", (char*)"-100"},
    {(char*)"SSD.exe", (char*)"R", (char*)"124872424232118782323422"},
  };

  const std::vector<std::array<char*, 4>> VALID_WRITE_ARGV_LIST = {
    {(char*)"SSD.exe", (char*)"W", (char*)"13", (char*)"0x12345678"},
    {(char*)"SSD.exe", (char*)"W", (char*)"13", (char*)"0x55565678"},
    {(char*)"SSD.exe", (char*)"W", (char*)"24", (char*)"0x12335678"},
    {(char*)"SSD.exe", (char*)"W", (char*)"12", (char*)"0XDEADBeeF"},
    {(char*)"SSD.exe", (char*)"W", (char*)"23", (char*)"0xDeaDBEE1"},
    {(char*)"SSD.exe", (char*)"W", (char*)"24", (char*)"0xDeaDBEE1"},
    {(char*)"SSD.exe", (char*)"W", (char*)"25", (char*)"0xDeaDBEE1"},
    {(char*)"SSD.exe", (char*)"W", (char*)"26", (char*)"0xDeaDBEE1"},
    {(char*)"SSD.exe", (char*)"W", (char*)"27", (char*)"0xDeaDBEE1"},
    {(char*)"SSD.exe", (char*)"W", (char*)"28", (char*)"0xDeaDBEE1"},
    {(char*)"SSD.exe", (char*)"W", (char*)"28", (char*)"0xDeaDBEE1"},
  };

  const std::vector<std::array<char*, 4>> INVALID_WRITE_ARGV_LIST = {
    {(char*)"SSD.exe", (char*)"W", (char*)"-1", (char*)"0x12345678"},
    {(char*)"SSD.exe", (char*)"W", (char*)"-100", (char*)"0x55562258"},
    {(char*)"SSD.exe", (char*)"W", (char*)"10", (char*)"0x12678"},
    {(char*)"SSD.exe", (char*)"W", (char*)"123", (char*)"0xDWADBEEF"},
    {(char*)"SSD.exe", (char*)"W", (char*)"11234", (char*)"0xDQADBEE1"},
    {(char*)"SSD.exe", (char*)"W", (char*)"11234", (char*)"1234567882"},
  };

  const std::vector<std::array<char*, 4>> VALID_ERASE_ARGV_LIST = {
    {(char*)"SSD.exe", (char*)"E", (char*)"13", (char*)"10"},
    {(char*)"SSD.exe", (char*)"E", (char*)"13", (char*)"1"},
    {(char*)"SSD.exe", (char*)"E", (char*)"24", (char*)"0"},
    {(char*)"SSD.exe", (char*)"E", (char*)"12", (char*)"3"},
    {(char*)"SSD.exe", (char*)"E", (char*)"23", (char*)"6"},
    {(char*)"SSD.exe", (char*)"E", (char*)"24", (char*)"5"},
    {(char*)"SSD.exe", (char*)"E", (char*)"25", (char*)"1"},
    {(char*)"SSD.exe", (char*)"E", (char*)"26", (char*)"3"},
    {(char*)"SSD.exe", (char*)"E", (char*)"27", (char*)"4"},
    {(char*)"SSD.exe", (char*)"E", (char*)"28", (char*)"8"},
    {(char*)"SSD.exe", (char*)"E", (char*)"99", (char*)"1"},
  };

  const std::vector<std::array<char*, 4>> INVALID_ERASE_ARGV_LIST = {
    {(char*)"SSD.exe", (char*)"E", (char*)"-1", (char*)"10"},
    {(char*)"SSD.exe", (char*)"E", (char*)"-100", (char*)"5"},
    {(char*)"SSD.exe", (char*)"E", (char*)"62", (char*)"-1"},
  };

  const std::vector<std::array<char*, 4>> ERROR_OUTPUT_ERASE_ARGV_LIST = {
    {(char*)"SSD.exe", (char*)"E", (char*)"99", (char*)"4"},
    {(char*)"SSD.exe", (char*)"E", (char*)"10", (char*)"16"},
    {(char*)"SSD.exe", (char*)"E", (char*)"42", (char*)"11"},
  };
};

TEST_F(SSD_IT_Fixture, SSD_IT_READ_INVALID_ARGC)
{
  for (auto testcase : VALID_READ_ARGV_LIST)
  {
    char** argv = const_cast<char**>(testcase.data());
    EXPECT_THROW(host->Execute(INVALID_READ_ARGC, argv), std::exception);
  }
}

TEST_F(SSD_IT_Fixture, SSD_IT_WRITE_INVALID_ARGC)
{
  for (auto testcase : VALID_WRITE_ARGV_LIST)
  {
    char** argv = const_cast<char**>(testcase.data());
    EXPECT_THROW(host->Execute(INVALID_WRITE_ARGC, argv), std::exception);
  }
}

TEST_F(SSD_IT_Fixture, SSD_IT_ERASE_INVALID_ARGC)
{
  for (auto testcase : VALID_ERASE_ARGV_LIST)
  {
    char** argv = const_cast<char**>(testcase.data());
    EXPECT_THROW(host->Execute(INVALID_ERASE_ARGC, argv), std::exception);
  }
}

TEST_F(SSD_IT_Fixture, SSD_IT_READ_INVALID_ARGV)
{
  for (auto testcase : INVALID_READ_ARGV_LIST)
  {
    char** argv = const_cast<char**>(testcase.data());
    EXPECT_THROW(host->Execute(VALID_READ_ARGC, argv), std::exception);
  }
}

TEST_F(SSD_IT_Fixture, SSD_IT_WRITE_INVALID_ARGV)
{
  for (auto testcase : INVALID_WRITE_ARGV_LIST)
  {
    char** argv = const_cast<char**>(testcase.data());
    EXPECT_THROW(host->Execute(VALID_WRITE_ARGC, argv), std::exception);
  }
}

TEST_F(SSD_IT_Fixture, SSD_IT_ERASE_INVALID_ARGV)
{
  for (auto testcase : INVALID_ERASE_ARGV_LIST)
  {
    char** argv = const_cast<char**>(testcase.data());
    EXPECT_THROW(host->Execute(VALID_ERASE_ARGC, argv), std::exception);
  }
}

TEST_F(SSD_IT_Fixture, SSD_IT_ERASE_ERROR_ARGV)
{
  for (auto testcase : ERROR_OUTPUT_ERASE_ARGV_LIST)
  {
    char** argv = const_cast<char**>(testcase.data());
    EXPECT_NO_THROW(host->Execute(VALID_ERASE_ARGC, argv));
    GlobalUtil::ValidateOutputDataWith(ERROR_MSG);
  }
}

TEST_F(SSD_IT_Fixture, SSD_IT_READ_VALID_ARGV)
{
  for (auto testcase : VALID_READ_ARGV_LIST)
  {
    char** argv = const_cast<char**>(testcase.data());
    EXPECT_NO_THROW(host->Execute(VALID_READ_ARGC, argv));
  }
}

TEST_F(SSD_IT_Fixture, SSD_IT_WRITE_VALID_ARGV)
{
  for (auto testcase : VALID_WRITE_ARGV_LIST)
  {
    char** argv = const_cast<char**>(testcase.data());
    EXPECT_NO_THROW(host->Execute(VALID_WRITE_ARGC, argv));
  }
}

TEST_F(SSD_IT_Fixture, SSD_IT_ERASE_VALID_ARGV)
{
  for (auto testcase : VALID_ERASE_ARGV_LIST)
  {
    char** argv = const_cast<char**>(testcase.data());
    EXPECT_NO_THROW(host->Execute(VALID_ERASE_ARGC, argv));
  }
}

#endif