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

  const LBA validLba = 5;
  const LBA invalidLba = 101;
  const DATA writeData = 0xdeadcafe;
  const DATA InvalidData = 0x0;
  const std::string outputFile = "ssd_output.txt";

  std::string getDataFromOutputFile(void)
  {
    std::ifstream ifs(outputFile);
    std::string readData((std::istreambuf_iterator<char>(ifs)),
      std::istreambuf_iterator<char>());
    ifs.close();

    return readData;
  }
};

TEST_F(MockNandSSDFixture, ReadAfterWrite)
{
  ON_CALL(mockNand, Read(_, _))
    .WillByDefault(Invoke([=](const LBA, DATA& out) -> bool
  {
    out = writeData;
    return true;
  }));

  ssd.write(validLba, writeData);

  ssd.read(validLba);
  std::string readData = getDataFromOutputFile();

  EXPECT_EQ((writeData), (stoul(readData)));
}

TEST_F(MockNandSSDFixture, ReadWithoutWrite)
{
  ON_CALL(mockNand, Read(_, _))
    .WillByDefault(Invoke([=](const LBA lba, DATA& out) -> bool
  {
    out = InvalidData;
    return true;
  }));

  ssd.read(validLba);
  std::string readData = getDataFromOutputFile();

  EXPECT_EQ(InvalidData, stoul(readData));
}

TEST_F(MockNandSSDFixture, ReadInvalidParam)
{
  EXPECT_THROW(ssd.read(invalidLba), std::exception);
}

TEST_F(MockNandSSDFixture, WriteInvalidParam)
{
  EXPECT_THROW(std::to_string(ssd.write(invalidLba, writeData)), std::exception);
}
#endif