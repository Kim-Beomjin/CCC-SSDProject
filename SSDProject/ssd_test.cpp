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
  MOCK_METHOD(bool, Read, (const LBA lba, DATA& out));
  MOCK_METHOD(bool, Write, (const LBA lba, const DATA data));
};

TEST(TC, ReadAfterWrite)
{
  MockNand* mockNand = new MockNand();
  SSD ssd(mockNand);
  LBA lba = 5;
  DATA writeData = 0xdeadcafe;
  std::string outputFile = "ssd_output.txt";

  ON_CALL(*mockNand, Read(_, _))
    .WillByDefault(Invoke([=](const LBA, DATA& out) -> bool
  {
    out = writeData;
    return true;
  }));

  ssd.write(lba, writeData);


  ssd.read(lba);
  
  std::ifstream ifs(outputFile);

  std::string readData((std::istreambuf_iterator<char>(ifs)),
                    std::istreambuf_iterator<char>());
  ifs.close();

  EXPECT_EQ((writeData), (stoul(readData)));

  delete mockNand;
}

TEST(TC, ReadWithoutWrite)
{
  MockNand* mockNand = new MockNand();
  SSD ssd(mockNand);
  LBA lba = 5;
  DATA expectData = 0x0;
  std::string outputFile = "ssd_output.txt";

  ON_CALL(*mockNand, Read(_, _))
    .WillByDefault(Invoke([=](const LBA lba, DATA& out) -> bool
  {
    out = expectData;
    return true;
  }));
  ssd.read(lba);
  
  std::ifstream ifs(outputFile);
  
  std::string readData((std::istreambuf_iterator<char>(ifs)),
                    std::istreambuf_iterator<char>());
  ifs.close();

  EXPECT_EQ(expectData, stoul(readData));

  delete mockNand;
}

TEST(TC, ReadInvalidParam)
{
  MockNand* mockNand = new MockNand();
  SSD ssd(mockNand);
  LBA lba = 101;

  EXPECT_THROW(ssd.read(lba), std::exception);

  delete mockNand;
}

TEST(TC, WriteInvalidParam)
{
  MockNand* mockNand = new MockNand();
  SSD ssd(mockNand);
  LBA lba = 101;
  DATA writeData = 0xdeadcafe;

  EXPECT_THROW(std::to_string(ssd.write(lba, writeData)), std::exception);

  delete mockNand;
}
#endif