#ifdef _DEBUG
#include "gmock/gmock.h"
#include "ssd.h"

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
  const DATA WRITE_DATA = 0xdeadcafe;
  const std::string outputFile = "ssd_output.txt";

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
    out = WRITE_DATA;
    return true;
  }));

  ssd.write(VALID_LBA, WRITE_DATA);

  ssd.read(VALID_LBA);
  std::string readData = getDataFromOutputFile();

  EXPECT_EQ((WRITE_DATA), (stoul(readData)));
}

TEST_F(MockNandSSDFixture, ReadWithoutWrite)
{
  ON_CALL(mockNand, Read(_, _))
    .WillByDefault(Invoke([=](const LBA lba, DATA& out) -> bool
  {
    out = EMPTY_DATA;
    return true;
  }));

  ssd.read(VALID_LBA);
  std::string readData = getDataFromOutputFile();

  EXPECT_EQ(EMPTY_DATA, stoul(readData));
}

TEST_F(MockNandSSDFixture, ReadInvalidParam)
{
  ssd.read(INVALID_LBA);
  std::string outputData = getDataFromOutputFile();

  EXPECT_EQ(ERROR_MSG, outputData);
}

TEST_F(MockNandSSDFixture, WriteInvalidParam)
{
  ssd.write(INVALID_LBA, WRITE_DATA);
  std::string outputData = getDataFromOutputFile();

  EXPECT_EQ(ERROR_MSG, outputData);
}
#endif