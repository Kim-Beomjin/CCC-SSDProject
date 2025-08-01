#ifdef _DEBUG
#include "gmock/gmock.h"
#include "hostInterface.h"
#include <string>
#include <vector>

class SSD_IT_Fixture : public testing::Test
{
public:
  void SetUp() override
  {
    host->SetSSD(new SSD(new Nand()));
  }

protected:
  HostInterface* host = HostInterface::GetInstance();

  static const int VALID_READ_ARGC = 3;
  static const int VALID_WRITE_ARGC = 4;
  static const int VALID_ERASE_ARGC = 4;
  static const int INVALID_READ_ARGC = 5;
  static const int INVALID_WRITE_ARGC = 3;
  static const int INVALID_ERASE_ARGC = 3;

  const std::vector<std::string> VALID_LBA_LIST = {
    "59", "99", "13", "13", "24", "12", "23", "24", "25", "26", "27", "28", "28"
  };

  const std::vector<std::string> INVALID_LBA_LIST = {
    "-1", "-100", "-152"
  };

  const std::vector<std::string> ERROR_LBA_LIST = {
    "12553", "124872322123122", "100"
  };
 
  const std::vector<std::string> VALID_WRITE_DATA_LIST = {
    "0x12345678", "0x55565678", "0x12335678", "0XDEADBeeF", "0xDeaDBEE1", "0xDeaDBEE1",
    "0xDeaDBEE1", "0xDeaDBEE1", "0xDeaDBEE1", "0xDeaDBEE1", "0xDeaDBEE1"
  };

  const std::vector<std::string> INVALID_WRITE_LBA_LIST = {
    "-1", "-100", "10", "123", "11234", "11234"
  };

  const std::vector<std::string> INVALID_WRITE_DATA_LIST = {
    "0x12678", "0xDWADBEEF", "0xDQADBEE1", "1234567882"
  };

  const std::vector<std::pair<std::string, std::string>> VALID_ERASE_ARGV_LIST = {
    {"13", "10"}, {"13", "1"}, {"24", "0"}, {"12", "3"}, {"23", "6"},
    {"24", "5"}, {"25", "1"}, {"26", "3"}, {"27", "4"}, {"28", "8"}, {"99", "1"}
  };

  const std::vector<std::pair<std::string, std::string>> INVALID_ERASE_ARGV_LIST = {
    {"-1", "10"}, {"-100", "5"}, {"62", "-1"}
  };

  const std::vector<std::pair<std::string, std::string>> ERROR_OUTPUT_ERASE_ARGV_LIST = {
    {"99", "4"}, {"10", "16"}, {"42", "11"}
  };

  void MakeArgv(std::vector<std::string>& args, std::vector<char*>& argv)
  {
    argv.clear();
    for (auto& str : args)
      argv.push_back((char*)str.data());
  }
};

TEST_F(SSD_IT_Fixture, SSD_IT_READ_INVALID_ARGC)
{
  for (const auto& lba : VALID_LBA_LIST)
  {
    std::vector<std::string> args = {"SSD.exe", "R", lba};
    std::vector<char*> argv;
    MakeArgv(args, argv);
    EXPECT_THROW(host->Execute(INVALID_READ_ARGC, argv.data()), std::exception);
  }
}

TEST_F(SSD_IT_Fixture, SSD_IT_WRITE_INVALID_ARGC)
{
  for (const auto& lba : VALID_LBA_LIST)
  {
    std::vector<std::string> args = {"SSD.exe", "W", lba};
    std::vector<char*> argv;
    MakeArgv(args, argv);
    EXPECT_THROW(host->Execute(INVALID_WRITE_ARGC, argv.data()), std::exception);
  }
}

TEST_F(SSD_IT_Fixture, SSD_IT_ERASE_INVALID_ARGC)
{
  for (const auto& p : VALID_ERASE_ARGV_LIST)
  {
    std::vector<std::string> args = {"SSD.exe", "E", p.first, p.second};
    std::vector<char*> argv;
    MakeArgv(args, argv);
    EXPECT_THROW(host->Execute(INVALID_ERASE_ARGC, argv.data()), std::exception);
  }
}

TEST_F(SSD_IT_Fixture, SSD_IT_READ_INVALID_ARGV)
{
  for (const auto& lba : INVALID_LBA_LIST)
  {
    std::vector<std::string> args = {"SSD.exe", "R", lba};
    std::vector<char*> argv;
    MakeArgv(args, argv);
    EXPECT_THROW(host->Execute(VALID_READ_ARGC, argv.data()), std::exception);
  }
}

TEST_F(SSD_IT_Fixture, SSD_IT_WRITE_INVALID_ARGV)
{
  for (int index = 0; index < INVALID_LBA_LIST[index].count; index++)
  {
    std::vector<std::string> args = {"SSD.exe", "W", INVALID_LBA_LIST[index], };
    std::vector<char*> argv;
    MakeArgv(args, argv);
    EXPECT_THROW(host->Execute(VALID_READ_ARGC, argv.data()), std::exception);
  }
}

TEST_F(SSD_IT_Fixture, SSD_IT_READ_ERROR_ARGV)
{
  for (const auto& lba : INVALID_LBA_LIST)
  {
    std::vector<std::string> args = {"SSD.exe", "R", lba};
    std::vector<char*> argv;
    MakeArgv(args, argv);
    EXPECT_THROW(host->Execute(VALID_READ_ARGC, argv.data()), std::exception);
  }
}

TEST_F(SSD_IT_Fixture, SSD_IT_ERASE_INVALID_ARGV)
{
  size_t max_size = std::min(INVALID_ERASE_ARGV_LIST.size(), VALID_ERASE_ARGV_LIST.size());
  for (size_t i = 0; i < max_size; ++i)
  {
    const auto& p = INVALID_ERASE_ARGV_LIST[i];
    std::vector<std::string> args = {"SSD.exe", "E", p.first, p.second};
    std::vector<char*> argv;
    MakeArgv(args, argv);
    EXPECT_THROW(host->Execute(VALID_ERASE_ARGC, argv.data()), std::exception);
  }
}

TEST_F(SSD_IT_Fixture, SSD_IT_ERASE_ERROR_ARGV)
{
  for (const auto& p : ERROR_OUTPUT_ERASE_ARGV_LIST)
  {
    std::vector<std::string> args = {"SSD.exe", "E", p.first, p.second};
    std::vector<char*> argv;
    MakeArgv(args, argv);
    EXPECT_NO_THROW(host->Execute(VALID_ERASE_ARGC, argv.data()));
    GlobalUtil::ValidateOutputDataWith(ERROR_MSG);
  }
}

TEST_F(SSD_IT_Fixture, SSD_IT_READ_VALID_ARGV)
{
  for (int i = 0; i < 3; ++i)
  {
    std::vector<std::string> args = {"SSD.exe", "R", VALID_LBA_LIST[i]};
    std::vector<char*> argv;
    MakeArgv(args, argv);
    EXPECT_NO_THROW(host->Execute(VALID_READ_ARGC, argv.data()));
  }
}

TEST_F(SSD_IT_Fixture, SSD_IT_WRITE_VALID_ARGV)
{
  for (size_t i = 0; i < VALID_WRITE_DATA_LIST.size(); ++i)
  {
    std::string lba = VALID_LBA_LIST[3 + i];
    std::string data = VALID_WRITE_DATA_LIST[i];
    std::vector<std::string> args = {"SSD.exe", "W", lba, data};
    std::vector<char*> argv;
    MakeArgv(args, argv);
    EXPECT_NO_THROW(host->Execute(VALID_WRITE_ARGC, argv.data()));
  }
}

TEST_F(SSD_IT_Fixture, SSD_IT_ERASE_VALID_ARGV)
{
  for (const auto& p : VALID_ERASE_ARGV_LIST)
  {
    std::vector<std::string> args = {"SSD.exe", "E", p.first, p.second};
    std::vector<char*> argv;
    MakeArgv(args, argv);
    EXPECT_NO_THROW(host->Execute(VALID_ERASE_ARGC, argv.data()));
  }
}

#endif
