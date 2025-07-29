#ifdef _DEBUG
#include "gmock/gmock.h"
#include "nand.h"
#include <vector>

class NandFixture : public testing::Test {
protected:
    class Nand nand;
    
    int readData = 0;
    static const int TEST_SIZE = 5;
    const std::vector<int> VALID_LBA_ADDR_LIST = { 0, 50, 70, 90, 99 };
    const std::vector<int> INVALID_LBA_ADDR_LIST = { -100, -10, -1, 100, 10000 };
    const int WRITE_DATA = 0xFFFF;
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

#endif