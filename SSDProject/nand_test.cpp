#ifdef _DEBUG
#include "gmock/gmock.h"
#include "nand.h"

class NandFixture : public testing::Test {
public:
    class Nand nand;
};

TEST_F(NandFixture, READ_INVALID_PARAMETER) {
    int lba = 10000; // lba must be 0 to 99,
    int out = 0;
    EXPECT_THROW(nand.read(lba, out), std::exception);
}

TEST_F(NandFixture, READ_VALID_PARAMETER)
{
    int lba = 50; //lba must be below 50
    int out = 0;
    EXPECT_EQ(nand.read(lba, out), true);
}

TEST_F(NandFixture, WRITE_INVALID_PARAMETER) {
    int lba = 10000; // lba must be 0 to 99,
    int in = 0xFFFF;
    EXPECT_THROW(nand.write(lba, in), std::exception);
}

TEST_F(NandFixture, WRITE_VALID_PARAMETER)
{
    int lba = 50; //lba must be below 50
    int in = 0xFFFF;
    EXPECT_EQ(nand.read(lba, in), true);
}

#endif