#ifdef _DEBUG
#include "gmock/gmock.h"
#include "nand.h"

class NandFixture : public testing::Test {
public:
    class Nand nand;
    
    int readData = 0;
    const int VALID_LBA_ADDR = 50;
    const int INVALID_LBA_ADDR = 10000;
    const int WRITE_DATA = 0xFFFF;

};

TEST_F(NandFixture, READ_VALID_AND_INVALID_PARAMETER_TEST) {
    EXPECT_THROW(nand.Read(INVALID_LBA_ADDR, readData), std::exception);
    EXPECT_EQ(nand.Read(VALID_LBA_ADDR, readData), true);
}

TEST_F(NandFixture, WRITE_VALID_AND_INVALID_PARAMETER_TEST) {
    EXPECT_THROW(nand.Write(INVALID_LBA_ADDR, WRITE_DATA), std::exception);
    EXPECT_EQ(nand.Write(VALID_LBA_ADDR, WRITE_DATA), true);
}


#endif