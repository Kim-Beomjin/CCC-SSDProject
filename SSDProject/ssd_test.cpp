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
  MOCK_METHOD(bool, Erase, (const LBA lba, const int size), (override));
};

class MockNandSSDFixture : public Test
{
public:
  NiceMock<MockNand> mockNand;
  SSD ssd{ &mockNand };

  const LBA VALID_LBA = 5;
  const LBA INVALID_LBA = 101;
  const std::string WRITE_DATA = "0x001F2DC0";
  const std::string INVALID_DATA = "0x00000000";

  void SetUp() override
  {
    _RemoveOutputFile();
  }

  std::string GetDataFromOutputFile(void)
  {
    std::ifstream ifs(OUTPUT_FILE);
    std::string readData((std::istreambuf_iterator<char>(ifs)),
      std::istreambuf_iterator<char>());

    return readData;
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
  void ValidateOutputDataWith(const std::string expectedData)
  {
    std::string outputData = GetDataFromOutputFile();
    EXPECT_EQ(expectedData, outputData);
  }
private:
  void _RemoveOutputFile(void)
  {
    int result = std::remove(OUTPUT_FILE.c_str());
    if (result != 0)
    {
      std::perror("file remove fail");
    }
  }
};

TEST_F(MockNandSSDFixture, ReadAfterWrite)
{
  ssd.Write(VALID_LBA, stoul(WRITE_DATA, nullptr, 16));
  ReadAndUpdateOutputFile(VALID_LBA, WRITE_DATA);

  ValidateOutputDataWith(WRITE_DATA);
}

TEST_F(MockNandSSDFixture, ReadWithoutWrite)
{
  ReadAndUpdateOutputFile(VALID_LBA, INVALID_DATA);

  ValidateOutputDataWith(INVALID_DATA);
}

TEST_F(MockNandSSDFixture, EraseLba)
{
  ssd.Write(VALID_LBA, stoul(WRITE_DATA, nullptr, 16));
  ReadAndUpdateOutputFile(VALID_LBA, WRITE_DATA);
  //ValidateOutputDataWith(WRITE_DATA);

  ssd.Erase(VALID_LBA, 1);
  ReadAndUpdateOutputFile(VALID_LBA, "0x00000000" /*No_data*/);
  ValidateOutputDataWith("0x00000000" /*No_data*/);
}

TEST_F(MockNandSSDFixture, ReadInvalidParam)
{
  ssd.Read(INVALID_LBA);

  ValidateOutputDataWith(ERROR_MSG);
}

TEST_F(MockNandSSDFixture, WriteInvalidParam)
{
  ssd.Write(INVALID_LBA, stoul(WRITE_DATA, nullptr, 16));

  ValidateOutputDataWith(ERROR_MSG);
}

TEST_F(MockNandSSDFixture, EraseInvalidLBA)
{
  ssd.Erase(INVALID_LBA, 6);

  ValidateOutputDataWith(ERROR_MSG);
}

TEST_F(MockNandSSDFixture, EraseInvalidSize)
{
  ssd.Erase(VALID_LBA, 11);

  ValidateOutputDataWith(ERROR_MSG);
}
#endif