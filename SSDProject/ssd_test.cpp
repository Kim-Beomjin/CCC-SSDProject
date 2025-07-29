#ifdef _DEBUG
#include "gmock/gmock.h"
#include "ssd.h"
#include <iostream>
#include <fstream>
#include <string>

TEST(TC, ReadAfterWrite)
{
  SSD ssd;
  unsigned int lba = 5;
  unsigned int writeData = 0xdeadcafe;
  std::string outputFile = "ssd_output.txt";

  ssd.write(lba, writeData);
  ssd.read(lba);
  
  std::ifstream ifs(outputFile);
  
  std::string readData((std::istreambuf_iterator<char>(ifs)),
                    std::istreambuf_iterator<char>());
  ifs.close();

  EXPECT_EQ(writeData, readData);
}

TEST(TC, ReadWithoutWrite)
{
  SSD ssd;
  unsigned int lba = 5;
  unsigned int expectData = 0x0;
  std::string outputFile = "ssd_output.txt";

  ssd.read(lba);
  
  std::ifstream ifs(outputFile);
  
  std::string readData((std::istreambuf_iterator<char>(ifs)),
                    std::istreambuf_iterator<char>());
  ifs.close();

  EXPECT_EQ(expectData, readData);
}

TEST(TC, ReadInvalidParam)
{
  SSD ssd;
  unsigned int lba = 101;

  EXPECT_THROW(ssd.read(lba), std::exception);
}

TEST(TC, WriteInvalidParam)
{
  SSD ssd;
  unsigned int lba = 101;
  unsigned int writeData = 0xdeadcafe;

  EXPECT_THROW(ssd.write(lba, writeData), std::exception);
}
#endif