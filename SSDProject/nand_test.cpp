#ifdef _DEBUG
#include "gmock/gmock.h"
#include "nand.h"
#include <vector>
#include <fstream>

class NandFixture : public testing::Test {
protected:
    bool IsFileExist(const std::string& filename) 
    {
        std::ifstream file(filename);
        return file.good();
    }
    void DeleteFile(const std::string& filename)
    {
        std::ifstream file(filename);
        if (file.good()) {
            file.close();
            std::remove(filename.c_str());
        }
    }
    class Nand nand;
    
    DATA readData = 0;
    static const int TEST_SIZE = 5;
    const std::vector<int> VALID_LBA_ADDR_LIST = { 0, 50, 70, 90, 99 };
    const std::vector<int> VALID_ERASE_LBA_ADDR_LIST = { 0, 50, 70, 90, 99 };
    const std::vector<int> VALID_ERASE_SIZE_LIST = { 5, 2, 10, 9, 1 };
    const std::vector<int> INVALID_LBA_ADDR_LIST = { -100, -10, -1, 100, 10000 };
    const DATA WRITE_DATA = 0xFFFF;
    const DATA OVERWRITE_DATA = 0xCCCCC;
    const int LARGE_ERASE_SIZE = 6;
    const int ERASE_SINGLE_LBA = 1;
};

TEST_F(NandFixture, READ_VALID_PARAMETER_TEST) {
    for (int lba : VALID_LBA_ADDR_LIST) {
        EXPECT_EQ(nand.Read(lba, readData), true);
    }
}

TEST_F(NandFixture, READ_INVALID_PARAMETER_TEST) {
    for (int lba : INVALID_LBA_ADDR_LIST) {
        EXPECT_THROW(nand.Read(lba, readData), std::exception);
    }
}

TEST_F(NandFixture, WRITE_VALID_PARAMETER_TEST) {
    for (int lba : VALID_LBA_ADDR_LIST) {
        EXPECT_EQ(nand.Write(lba, WRITE_DATA), true);
    }
}

TEST_F(NandFixture, WRITE_INVALID_PARAMETER_TEST) {
    for (int lba : INVALID_LBA_ADDR_LIST) {
        EXPECT_THROW(nand.Write(lba, WRITE_DATA), std::exception);
    }
}

TEST_F(NandFixture, ERASE_VALID_PARAMETER_TEST) {
  for (int index = 0; index < TEST_SIZE; index++)
  {
      EXPECT_EQ(nand.Erase(VALID_ERASE_LBA_ADDR_LIST[index], VALID_ERASE_SIZE_LIST[index]), true);
  }
}

TEST_F(NandFixture, ERASE_INVALID_PARAMETER_TEST) {
    for (int lba : INVALID_LBA_ADDR_LIST) {
        EXPECT_THROW(nand.Erase(lba, LARGE_ERASE_SIZE), std::exception);
    }
}

TEST_F(NandFixture, FILE_EXIST_AFTER_READ) {
    DeleteFile(NAND_FILE_NAME);
    for (int lba : VALID_LBA_ADDR_LIST) {
        nand.Read(lba, readData);
    }
    EXPECT_EQ(IsFileExist(NAND_FILE_NAME), true);
}

TEST_F(NandFixture, FILE_EXIST_AFTER_WRITE) {
    DeleteFile(NAND_FILE_NAME);
    for (int lba : VALID_LBA_ADDR_LIST) {
        nand.Write(lba, WRITE_DATA);
    }
    EXPECT_EQ(IsFileExist(NAND_FILE_NAME), true);
}

TEST_F(NandFixture, FILE_EXIST_AFTER_ERASE) {
    DeleteFile(NAND_FILE_NAME);
    for (int lba : VALID_LBA_ADDR_LIST) {
        nand.Erase(lba, ERASE_SINGLE_LBA);
    }
    EXPECT_EQ(IsFileExist(NAND_FILE_NAME), true);
}

TEST_F(NandFixture, READ_AFTER_WRITE)
{
    DeleteFile(NAND_FILE_NAME);
    for (int lba : VALID_LBA_ADDR_LIST) {
        nand.Write(lba, WRITE_DATA);
    }
    for (int lba : VALID_LBA_ADDR_LIST) {
        nand.Read(lba, readData);
        EXPECT_EQ(readData, WRITE_DATA);
    }
}

TEST_F(NandFixture, READ_WITHOUT_WRITE)
{
    DeleteFile(NAND_FILE_NAME);
    for (int lba : VALID_LBA_ADDR_LIST) {
        nand.Read(lba, readData);
        EXPECT_EQ(readData, EMPTY_DATA);
    }
}

TEST_F(NandFixture, READ_AFTER_OVERWRITE)
{
    DeleteFile(NAND_FILE_NAME);
    for (int lba : VALID_LBA_ADDR_LIST) {
        nand.Write(lba, WRITE_DATA);
    }
    for (int lba : VALID_LBA_ADDR_LIST) {
        nand.Read(lba, readData);
        EXPECT_EQ(readData, WRITE_DATA);
    }
    for (int lba : VALID_LBA_ADDR_LIST) {
        nand.Write(lba, OVERWRITE_DATA);
    }
    for (int lba : VALID_LBA_ADDR_LIST) {
        nand.Read(lba, readData);
        EXPECT_EQ(readData, OVERWRITE_DATA);
    }
}

TEST_F(NandFixture, SINGLE_ERASE_AFTER_WRITE)
{
  DeleteFile(NAND_FILE_NAME);
  for (int lba : VALID_LBA_ADDR_LIST)
  {
    nand.Write(lba, WRITE_DATA);
  }
  for (int lba : VALID_LBA_ADDR_LIST)
  {
    nand.Read(lba, readData);
    EXPECT_EQ(readData, WRITE_DATA);
  }
  for (int lba : VALID_LBA_ADDR_LIST)
  {
    nand.Erase(lba, ERASE_SINGLE_LBA);
  }
  for (int lba : VALID_LBA_ADDR_LIST)
  {
    nand.Read(lba, readData);
    EXPECT_EQ(readData, EMPTY_DATA);
  }
}

TEST_F(NandFixture, LARGE_ERASE_AFTER_WRITE)
{
  DeleteFile(NAND_FILE_NAME);

  for (int index = 0; index < TEST_SIZE; index++)
  {
    int startLba = VALID_ERASE_LBA_ADDR_LIST[index];
    for (int lba = startLba; lba < startLba + VALID_ERASE_SIZE_LIST[index]; lba++)
    {
      nand.Write(lba, WRITE_DATA);
    }
  }
  for (int index = 0; index < TEST_SIZE; index++)
  {
    int startLba = VALID_ERASE_LBA_ADDR_LIST[index];
    for (int lba = startLba; lba < startLba + VALID_ERASE_SIZE_LIST[index]; lba++)
    {
      nand.Read(lba, readData);
      EXPECT_EQ(readData, WRITE_DATA);
    }
  }
  for (int index = 0; index < TEST_SIZE; index++)
  {
    nand.Erase(VALID_ERASE_LBA_ADDR_LIST[index], VALID_ERASE_SIZE_LIST[index]);
  }
  for (int index = 0; index < TEST_SIZE; index++)
  {
    int startLba = VALID_ERASE_LBA_ADDR_LIST[index];
    for (int lba = startLba; lba < startLba + VALID_ERASE_SIZE_LIST[index]; lba++)
    {
      nand.Read(lba, readData);
      EXPECT_EQ(readData, EMPTY_DATA);
    }
  }
}

#endif