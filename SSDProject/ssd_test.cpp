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
  MOCK_METHOD(bool, Read, (const int lba, int& out));
  MOCK_METHOD(bool, Write, (const int lba, const int data));
};

TEST(TC, ReadAfterWrite)
{
  MockNand* mockNand = new MockNand();
  SSD ssd(mockNand);
  int lba = 5;
  int writeData = 0xdeadcafe;
  std::string outputFile = "ssd_output.txt";

  ON_CALL(*mockNand, Read(_, _))
    .WillByDefault(Invoke([=](const int, int& out) -> bool
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

  EXPECT_EQ((writeData), (stoi(readData)));

  delete mockNand;
}

TEST(TC, ReadWithoutWrite)
{
  MockNand* mockNand = new MockNand();
  SSD ssd(mockNand);
  int lba = 5;
  int expectData = 0x0;
  std::string outputFile = "ssd_output.txt";

  ON_CALL(*mockNand, Read(_, _))
    .WillByDefault(Invoke([=](const int lba, int& out) -> bool
  {
    out = expectData;
    return true;
  }));
  ssd.read(lba);
  
  std::ifstream ifs(outputFile);
  
  std::string readData((std::istreambuf_iterator<char>(ifs)),
                    std::istreambuf_iterator<char>());
  ifs.close();

  EXPECT_EQ(expectData, stoi(readData));

  delete mockNand;
}

TEST(TC, ReadInvalidParam)
{
  MockNand* mockNand = new MockNand();
  SSD ssd(mockNand);
  int lba = 101;

  EXPECT_THROW(ssd.read(lba), std::exception);

  delete mockNand;
}

TEST(TC, WriteInvalidParam)
{
  MockNand* mockNand = new MockNand();
  SSD ssd(mockNand);
  int lba = 101;
  int writeData = 0xdeadcafe;

  EXPECT_THROW(std::to_string(ssd.write(lba, writeData)), std::exception);

  delete mockNand;
}
#endif