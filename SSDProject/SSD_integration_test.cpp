#ifdef _DEBUG
#include "gmock/gmock.h"
#include "hostInterface.h"
#include <string>
#include <vector>

class SSD_IT_Fixture : public testing::Test
{
public:
  void SetUp()
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

  const std::vector<char*> VALID_LBA_LIST = {
    (char*)"59",
    (char*)"99",
    (char*)"12",
    (char*)"13", (char*)"13", (char*)"24", (char*)"12", (char*)"23",
    (char*)"24", (char*)"25", (char*)"26", (char*)"27", (char*)"28", (char*)"28",
  };

  const std::vector<char*> INVALID_LBA_LIST = {
    (char*)"-1",
    (char*)"-100",
    (char*)"124872424232118782323422",
  };

  const std::vector<char*> VALID_WRITE_DATA_LIST = {
    (char*)"0x12345678", (char*)"0x55565678", (char*)"0x12335678",
    (char*)"0XDEADBeeF", (char*)"0xDeaDBEE1", (char*)"0xDeaDBEE1",
    (char*)"0xDeaDBEE1", (char*)"0xDeaDBEE1", (char*)"0xDeaDBEE1",
    (char*)"0xDeaDBEE1", (char*)"0xDeaDBEE1",
  };

  const std::vector<char*> INVALID_WRITE_LBA_LIST = {
    (char*)"-1", (char*)"-100", (char*)"10", (char*)"123", (char*)"11234", (char*)"11234",
  };

  const std::vector<char*> INVALID_WRITE_DATA_LIST = {
    (char*)"0x12678", (char*)"0xDWADBEEF", (char*)"0xDQADBEE1", (char*)"1234567882",
  };

  const std::vector<std::pair<char*, char*>> VALID_ERASE_ARGV_LIST = {
    {(char*)"13", (char*)"10"},
    {(char*)"13", (char*)"1"},
    {(char*)"24", (char*)"0"},
    {(char*)"12", (char*)"3"},
    {(char*)"23", (char*)"6"},
    {(char*)"24", (char*)"5"},
    {(char*)"25", (char*)"1"},
    {(char*)"26", (char*)"3"},
    {(char*)"27", (char*)"4"},
    {(char*)"28", (char*)"8"},
    {(char*)"99", (char*)"1"},
  };

  const std::vector<std::pair<char*, char*>> INVALID_ERASE_ARGV_LIST = {
    {(char*)"-1", (char*)"10"},
    {(char*)"-100", (char*)"5"},
    {(char*)"62", (char*)"-1"},
  };

  const std::vector<std::pair<char*, char*>> ERROR_OUTPUT_ERASE_ARGV_LIST = {
    {(char*)"99", (char*)"4"},
    {(char*)"10", (char*)"16"},
    {(char*)"42", (char*)"11"},
  };

  void MakeReadArgv(char** argv, char* lba)
  {
    argv[0] = (char*)"SSD.exe";
    argv[1] = (char*)"R";
    argv[2] = lba;
  }

  void MakeWriteArgv(char** argv, char* lba, char* data)
  {
    argv[0] = (char*)"SSD.exe";
    argv[1] = (char*)"W";
    argv[2] = lba;
    argv[3] = data;
  }

  void MakeEraseArgv(char** argv, const std::pair<char*, char*>& lba_count)
  {
    argv[0] = (char*)"SSD.exe";
    argv[1] = (char*)"E";
    argv[2] = lba_count.first;
    argv[3] = lba_count.second;
  }
};

TEST_F(SSD_IT_Fixture, SSD_IT_READ_INVALID_ARGC)
{
  char* argv[5];
  for (auto lba : VALID_LBA_LIST)
  {
    MakeReadArgv(argv, lba);
    EXPECT_THROW(host->Execute(INVALID_READ_ARGC, argv), std::exception);
  }
}

TEST_F(SSD_IT_Fixture, SSD_IT_WRITE_INVALID_ARGC)
{
  char* argv[5];
  for (int index = 0; index < VALID_WRITE_DATA_LIST.size(); index++)
  {
    MakeWriteArgv(argv, VALID_LBA_LIST[3 + index], VALID_WRITE_DATA_LIST[index]); // 3부터 VALID_WRITE_LBA 부분
    EXPECT_THROW(host->Execute(INVALID_WRITE_ARGC, argv), std::exception);
  }
}

TEST_F(SSD_IT_Fixture, SSD_IT_ERASE_INVALID_ARGC)
{
  char* argv[5];
  for (auto& p : VALID_ERASE_ARGV_LIST)
  {
    MakeEraseArgv(argv, p);
    EXPECT_THROW(host->Execute(INVALID_ERASE_ARGC, argv), std::exception);
  }
}

TEST_F(SSD_IT_Fixture, SSD_IT_READ_INVALID_ARGV)
{
  char* argv[3];
  for (auto lba : INVALID_LBA_LIST)
  {
    MakeReadArgv(argv, lba);
    EXPECT_THROW(host->Execute(VALID_READ_ARGC, argv), std::exception);
  }
}

TEST_F(SSD_IT_Fixture, SSD_IT_ERASE_INVALID_ARGV)
{
  char* argv[4];
  int max_size = std::min(INVALID_ERASE_ARGV_LIST.size(), VALID_ERASE_ARGV_LIST.size());
  for (int index = 0; index < max_size; index++)
  {
    MakeEraseArgv(argv, INVALID_ERASE_ARGV_LIST[index]);
    EXPECT_THROW(host->Execute(VALID_ERASE_ARGC, argv), std::exception);
  }
}

TEST_F(SSD_IT_Fixture, SSD_IT_ERASE_ERROR_ARGV)
{
  char* argv[4];
  for (auto& p : ERROR_OUTPUT_ERASE_ARGV_LIST)
  {
    MakeEraseArgv(argv, p);
    EXPECT_NO_THROW(host->Execute(VALID_ERASE_ARGC, argv));
    GlobalUtil::ValidateOutputDataWith(ERROR_MSG);
  }
}

TEST_F(SSD_IT_Fixture, SSD_IT_READ_VALID_ARGV)
{
  char* argv[3];
  for (int index = 0; index < 3; index++)
  {
    MakeReadArgv(argv, VALID_LBA_LIST[index]);
    EXPECT_NO_THROW(host->Execute(VALID_READ_ARGC, argv));
  }
}

TEST_F(SSD_IT_Fixture, SSD_IT_WRITE_VALID_ARGV)
{
  char* argv[4];
  for (int index = 0; index < VALID_WRITE_DATA_LIST.size(); index++)
  {
    MakeWriteArgv(argv, VALID_LBA_LIST[3 + index], VALID_WRITE_DATA_LIST[index]);
    EXPECT_NO_THROW(host->Execute(VALID_WRITE_ARGC, argv));
  }
}

TEST_F(SSD_IT_Fixture, SSD_IT_ERASE_VALID_ARGV)
{
  char* argv[4];
  for (auto& p : VALID_ERASE_ARGV_LIST)
  {
    MakeEraseArgv(argv, p);
    EXPECT_NO_THROW(host->Execute(VALID_ERASE_ARGC, argv));
  }
}

#endif
