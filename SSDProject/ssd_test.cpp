#ifdef _DEBUG
#include "gmock/gmock.h"
#include "ssd.h"
#include <cstdio>
#include <fstream>
#include <string>

using namespace testing;

using ::testing::_;
using ::testing::Invoke;
using ::testing::DoAll;
using ::testing::SetArgReferee;

class MockNand : public NandInterface
{
public:
  MOCK_METHOD(bool, Read, (const LBA lba, DATA& readData), (override));
  MOCK_METHOD(bool, Write, (const LBA lba, const DATA writeData), (override));
  MOCK_METHOD(bool, Erase, (const LBA lba, const ERASE_SIZE erase_size), (override));
};

class MockNandSSDFixture : public Test
{
public:
  NiceMock<MockNand> mockNand;
  SSD ssd{ &mockNand };

  const LBA VALID_LBA = 5;
  const LBA INVALID_LBA = 101;
  const std::string WRITE_DATA_STRING = "0x001F2DC0";
  const std::string EMPTY_DATA_STRING = "0x00000000";

  void SetUp() override
  {
    GlobalUtil::DBG_RemoveOutputFile();
  }

  void ReadAndUpdateOutputFile(LBA lba, const std::string writeData)
  {
    ON_CALL(mockNand, Read(_, _))
      .WillByDefault(Invoke([=](const LBA, DATA& readData) -> bool
    {
      readData = stoul(writeData, nullptr, 16);
      return true;
    }));

    ssd.Read(lba);
  }
};

TEST_F(MockNandSSDFixture, ReadAfterWrite)
{
  ssd.Write(VALID_LBA, stoul(WRITE_DATA_STRING, nullptr, 16));
  ReadAndUpdateOutputFile(VALID_LBA, WRITE_DATA_STRING);

  GlobalUtil::ValidateOutputDataWith(WRITE_DATA_STRING);
}

TEST_F(MockNandSSDFixture, ReadWithoutWrite)
{
  ReadAndUpdateOutputFile(VALID_LBA, EMPTY_DATA_STRING);

  GlobalUtil::ValidateOutputDataWith(EMPTY_DATA_STRING);
}

TEST_F(MockNandSSDFixture, EraseLba)
{
  ssd.Write(VALID_LBA, stoul(WRITE_DATA_STRING, nullptr, 16));
  ReadAndUpdateOutputFile(VALID_LBA, WRITE_DATA_STRING);
  //ValidateOutputDataWith(WRITE_DATA_STRING);

  ssd.Erase(VALID_LBA, 1);
  ReadAndUpdateOutputFile(VALID_LBA, EMPTY_DATA_STRING);
  GlobalUtil::ValidateOutputDataWith(EMPTY_DATA_STRING);
}

TEST_F(MockNandSSDFixture, ReadInvalidParam)
{
  ssd.Read(INVALID_LBA);

  GlobalUtil::ValidateOutputDataWith(ERROR_MSG);
}

TEST_F(MockNandSSDFixture, WriteInvalidParam)
{
  ssd.Write(INVALID_LBA, stoul(WRITE_DATA_STRING, nullptr, 16));

  GlobalUtil::ValidateOutputDataWith(ERROR_MSG);
}

TEST_F(MockNandSSDFixture, EraseInvalidLBA)
{
  ssd.Erase(INVALID_LBA, 6);

  GlobalUtil::ValidateOutputDataWith(ERROR_MSG);
}

TEST_F(MockNandSSDFixture, EraseInvalidSize)
{
  ssd.Erase(VALID_LBA, 11);

  GlobalUtil::ValidateOutputDataWith(ERROR_MSG);
}
#endif