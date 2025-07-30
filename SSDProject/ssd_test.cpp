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
  MOCK_METHOD(bool, Read, (const LBA lba, DATA& out), (override));
  MOCK_METHOD(bool, Write, (const LBA lba, const DATA data), (override));
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
  const std::string outputFile = "ssd_output.txt";

  void SetUp() override
  {
    int result = std::remove(outputFile.c_str());
    if (result != 0)
    {
      std::perror("파일 삭제 실패");
    }
  }

  std::string getDataFromOutputFile(void)
  {
    std::ifstream ifs(outputFile);
    std::string readData((std::istreambuf_iterator<char>(ifs)),
      std::istreambuf_iterator<char>());

    return readData;
  }
};

TEST_F(MockNandSSDFixture, ReadAfterWrite)
{
  ON_CALL(mockNand, Read(_, _))
    .WillByDefault(Invoke([=](const LBA, DATA& out) -> bool
  {
    out = stoul(WRITE_DATA, nullptr, 16);
    return true;
  }));

  ssd.write(VALID_LBA, stoul(WRITE_DATA, nullptr, 16));

  ssd.read(VALID_LBA);
  std::string readData = getDataFromOutputFile();

  EXPECT_EQ(WRITE_DATA, readData);
}

TEST_F(MockNandSSDFixture, ReadWithoutWrite)
{
  ON_CALL(mockNand, Read(_, _))
    .WillByDefault(Invoke([=](const LBA lba, DATA& out) -> bool
  {
    out = stoul(INVALID_DATA, nullptr, 16);
    return true;
  }));

  ssd.read(VALID_LBA);
  std::string readData = getDataFromOutputFile();

  EXPECT_EQ(INVALID_DATA, readData);
}

TEST_F(MockNandSSDFixture, ReadInvalidParam)
{
  ssd.read(INVALID_LBA);
  std::string outputData = getDataFromOutputFile();

  EXPECT_EQ(ERROR_MSG, outputData);
}

TEST_F(MockNandSSDFixture, WriteInvalidParam)
{
  ssd.write(INVALID_LBA, stoul(WRITE_DATA, nullptr, 16));
  std::string outputData = getDataFromOutputFile();

  EXPECT_EQ(ERROR_MSG, outputData);
}
#endif