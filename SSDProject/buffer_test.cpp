#ifdef _DEBUG
#include "gmock/gmock.h"
#include "hostInterface.h"
#include <string>
#include <vector>
#include <array>
#include <utility>

class BufferFixture : public testing::Test {
protected:
    void SetUp()
    {
        char** argv = const_cast<char**>(FLUSH_ARGV.data());
        HostInterface::GetInstance()->Execute(FLUSH_ARGC, argv);
        
    }
    BufferedSSD *ssd;
    const std::array<char*, 2> FLUSH_ARGV = { (char*)"SSD.exe", (char*)"F" };
    const int FLUSH_ARGC = 2;
    const int WRITE_ARGC = 4;
    const int ERASE_ARGC = 4;
    const int READ_ARGC = 3;

    void HostExecute(char** argv)
    {
        HostInterface* host = HostInterface::GetInstance();
        ssd = new BufferedSSD();
        host->SetSSD(ssd);
        if (argv[1] == "W") host->Execute(WRITE_ARGC, argv);
        if (argv[1] == "E") host->Execute(ERASE_ARGC, argv);
        if (argv[1] == "R") host->Execute(READ_ARGC, argv);
        if (argv[1] == "F") host->Execute(FLUSH_ARGC, argv);
    }

    const std::vector<std::pair<std::array<char*, 4>, int>> BUFFER_WRITE_ONLY_TEST_ARGV_LIST = {
        {{ (char*)"SSD.exe", (char*)"W", (char*)"13", (char*)"0x12345678" }, 1},
        {{ (char*)"SSD.exe", (char*)"W", (char*)"14", (char*)"0x12345678" }, 2}, 
        {{ (char*)"SSD.exe", (char*)"W", (char*)"13", (char*)"0x12345678" }, 2}, // overwrite case
        {{ (char*)"SSD.exe", (char*)"W", (char*)"14", (char*)"0x12345678" }, 2}, // overwrite case
        {{ (char*)"SSD.exe", (char*)"W", (char*)"15", (char*)"0x12345678" }, 3},
        {{ (char*)"SSD.exe", (char*)"W", (char*)"16", (char*)"0x12345678" }, 4},
        {{ (char*)"SSD.exe", (char*)"W", (char*)"17", (char*)"0x12345678" }, 5},
        {{ (char*)"SSD.exe", (char*)"W", (char*)"13", (char*)"0x12345678" }, 1}, // flush first
    };

    const std::vector<std::pair<std::array<char*, 4>, int>> BUFFER_ERASE_ONLY_TEST_ARGV_LIST = {
        {{ (char*)"SSD.exe", (char*)"E", (char*)"0",  (char*)"10" },  1},
        {{ (char*)"SSD.exe", (char*)"E", (char*)"10", (char*)"10" },  2},
        {{ (char*)"SSD.exe", (char*)"E", (char*)"0",  (char*)"10" },  2},
        {{ (char*)"SSD.exe", (char*)"E", (char*)"20", (char*)"10" },  3},
        {{ (char*)"SSD.exe", (char*)"E", (char*)"50", (char*)"2"  },  4}, 
        {{ (char*)"SSD.exe", (char*)"E", (char*)"52", (char*)"1"  },  4}, 
        {{ (char*)"SSD.exe", (char*)"E", (char*)"53", (char*)"3"  },  4},
        {{ (char*)"SSD.exe", (char*)"E", (char*)"56", (char*)"4"  },  4},
        {{ (char*)"SSD.exe", (char*)"E", (char*)"60", (char*)"1"  },  5},
        {{ (char*)"SSD.exe", (char*)"E", (char*)"61", (char*)"1"  },  1}, // flush first
    };

    const std::vector<std::pair<std::array<char*, 4>, int>> BUFFER_TEST_EXAMPLE_IGNORE_COMMAND = {
        {{ (char*)"SSD.exe", (char*)"W", (char*)"20", (char*)"0xABCDABCD" }, 1},
        {{ (char*)"SSD.exe", (char*)"W", (char*)"21", (char*)"0x12341234" }, 2},
        {{ (char*)"SSD.exe", (char*)"W", (char*)"20", (char*)"0xEEEEFFFF" }, 2}, 
        {{ (char*)"SSD.exe", (char*)"F", (char*)"  ", (char*)"          " }, 0}, 
        {{ (char*)"SSD.exe", (char*)"E", (char*)"18", (char*)         "3" }, 1},
        {{ (char*)"SSD.exe", (char*)"W", (char*)"21", (char*)"0x12341234" }, 2},
        {{ (char*)"SSD.exe", (char*)"E", (char*)"18", (char*)         "5" }, 1},
    };

    const std::vector<std::pair<std::array<char*, 4>, int>> BUFFER_TEST_EXAMPLE_MERGE_ERASE = {
        {{ (char*)"SSD.exe", (char*)"W", (char*)"20", (char*)"0xABCDABCD" }, 1},
        {{ (char*)"SSD.exe", (char*)"E", (char*)"10", (char*)         "4" }, 2},
        {{ (char*)"SSD.exe", (char*)"E", (char*)"12", (char*)         "3" }, 2},
    };

    const std::vector<std::pair<std::array<char*, 4>, int>> BUFFER_TEST_MIXED_WRITE_ERASE_1 = {
        {{ (char*)"SSD.exe", (char*)"E", (char*)"0",  (char*)        "10" }, 1},
        {{ (char*)"SSD.exe", (char*)"W", (char*)"10", (char*)"0xABCDABCD" }, 2},
        {{ (char*)"SSD.exe", (char*)"E", (char*)"11", (char*)        "10" }, 3},
        {{ (char*)"SSD.exe", (char*)"E", (char*)"21", (char*)         "3" }, 4},
        {{ (char*)"SSD.exe", (char*)"E", (char*)"24", (char*)        "10" }, 5},
        {{ (char*)"SSD.exe", (char*)"W", (char*)"13", (char*)"0xABCDABCD" }, 1},
        {{ (char*)"SSD.exe", (char*)"E", (char*)"10", (char*)         "3" }, 2},
        {{ (char*)"SSD.exe", (char*)"E", (char*)"14", (char*)         "6" }, 2},
        {{ (char*)"SSD.exe", (char*)"W", (char*)"14", (char*)"0xABCDABCD" }, 3},
        {{ (char*)"SSD.exe", (char*)"W", (char*)"15", (char*)"0xABCDABCD" }, 4},
        {{ (char*)"SSD.exe", (char*)"E", (char*)"13", (char*)         "3" }, 1},
    };

};

TEST_F(BufferFixture, WRITE_ONLY_BUFFER_TEST)
{
    for (auto test_case : BUFFER_WRITE_ONLY_TEST_ARGV_LIST)
    {
        char** argv = const_cast<char**>(test_case.first.data());
        HostExecute(argv);
        EXPECT_EQ(ssd->GetCmdCnt(), test_case.second);
    }
}

TEST_F(BufferFixture, ERASE_ONLY_BUFFER_TEST)
{
    for (auto test_case : BUFFER_ERASE_ONLY_TEST_ARGV_LIST)
    {
        char** argv = const_cast<char**>(test_case.first.data());
        HostExecute(argv);
        EXPECT_EQ(ssd->GetCmdCnt(), test_case.second);
    }
}

TEST_F(BufferFixture, IGNORE_COMNMAND_BUFFER_TEST)
{
    for (auto test_case : BUFFER_TEST_EXAMPLE_IGNORE_COMMAND)
    {
        char** argv = const_cast<char**>(test_case.first.data());
        HostExecute(argv);
        EXPECT_EQ(ssd->GetCmdCnt(), test_case.second);
    }
}

TEST_F(BufferFixture, MERGE_ERASE_BUFFER_TEST)
{
    for (auto test_case : BUFFER_TEST_EXAMPLE_MERGE_ERASE)
    {
        char** argv = const_cast<char**>(test_case.first.data());
        HostExecute(argv);
        EXPECT_EQ(ssd->GetCmdCnt(), test_case.second);
    }
}

TEST_F(BufferFixture, MIXED_BUFFER_TEST_1)
{
    for (auto test_case : BUFFER_TEST_MIXED_WRITE_ERASE_1)
    {
        char** argv = const_cast<char**>(test_case.first.data());
        HostExecute(argv);
        EXPECT_EQ(ssd->GetCmdCnt(), test_case.second);
    }
}

#endif