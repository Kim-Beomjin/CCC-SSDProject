#ifdef _DEBUG
#include "gmock/gmock.h"
#include "hostInterface.h"
#include <string>
#include <vector>
#include <array>

class MockNand : public NandInterface
{
public:
    MOCK_METHOD(bool, Read, (const LBA lba, DATA& out), (override));
    MOCK_METHOD(bool, Write, (const LBA lba, const DATA data), (override));
};

class HostInterfaceFixture : public testing::Test {
protected:
    testing::NiceMock<MockNand> mockNand;
    SSD MockSSD{ &mockNand };
    static const int VALID_READ_ARGC = 3;
    static const int VALID_WRITE_ARGC = 4;
    static const int INVALID_READ_ARGC = 5;
    static const int INVALID_WRITE_ARGC = 3;
    const std::vector<std::array<char*, 3>> VALID_READ_ARGV_LIST = {
        { (char*)"SSD.exe", (char*)"R", (char*)"11523" },
        { (char*)"SSD.exe", (char*)"R", (char*)"224" },
        { (char*)"SSD.exe", (char*)"R", (char*)"12" },
    };

    const std::vector<std::array<char*, 3>> INVALID_READ_ARGV_LIST = {
        { (char*)"SSD.exe", (char*)"R", (char*)"-1" },
        { (char*)"SSD.exe", (char*)"R", (char*)"-100" },
        { (char*)"SSD.exe", (char*)"R", (char*)"124872424232118782323422" },
    };

    const std::vector<std::array<char*, 4>> VALID_WRITE_ARGV_LIST = {
        { (char*)"SSD.exe", (char*)"W", (char*)"123", (char*)"0x12345678" },
        { (char*)"SSD.exe", (char*)"W", (char*)"11523", (char*)"0x55565678" },
        { (char*)"SSD.exe", (char*)"W", (char*)"224", (char*)"0x12335678" },
        { (char*)"SSD.exe", (char*)"W", (char*)"12", (char*)"0xDEADBeeF" },
        { (char*)"SSD.exe", (char*)"W", (char*)"23", (char*)"0xDeaDBEE1" },
    };

    const std::vector<std::array<char*, 4>> INVALID_WRITE_ARGV_LIST = {
     { (char*)"SSD.exe", (char*)"W", (char*)"-1",     (char*)"0x12345678" },
     { (char*)"SSD.exe", (char*)"W", (char*)"-100",   (char*)"0x55562258" },
     { (char*)"SSD.exe", (char*)"W", (char*)"10",     (char*)"0x12678" },
     { (char*)"SSD.exe", (char*)"W", (char*)"123",    (char*)"0xDWADBEEF" },
     { (char*)"SSD.exe", (char*)"W", (char*)"11234",  (char*)"0xDQADBEE1" },
     { (char*)"SSD.exe", (char*)"W", (char*)"11234",  (char*)"1234567882" },
    };
};

TEST_F(HostInterfaceFixture, READ_INVALID_ARGC)
{
    HostInterface* host = HostInterface::GetInstance();
    host->SetSSD(&MockSSD);

    for (auto testcase : VALID_READ_ARGV_LIST) {
        char** argv = const_cast<char**>(testcase.data());
        EXPECT_THROW(host->Execute(INVALID_READ_ARGC, argv), std::exception);
    }
}


TEST_F(HostInterfaceFixture, WRITE_INVALID_ARGC)
{
    HostInterface* host = HostInterface::GetInstance();
    host->SetSSD(&MockSSD);

    for (auto testcase : VALID_WRITE_ARGV_LIST) {
        char** argv = const_cast<char**>(testcase.data());
        EXPECT_THROW(host->Execute(INVALID_WRITE_ARGC, argv), std::exception);
    }
}

TEST_F(HostInterfaceFixture, READ_INVALID_ARGV)
{
    HostInterface* host = HostInterface::GetInstance();
    host->SetSSD(&MockSSD);

    for (auto testcase : INVALID_READ_ARGV_LIST) {
        char** argv = const_cast<char**>(testcase.data());
        EXPECT_THROW(host->Execute(VALID_READ_ARGC, argv), std::exception);
    }
}

TEST_F(HostInterfaceFixture, READ_VALID_ARGV)
{
    HostInterface* host = HostInterface::GetInstance();
    host->SetSSD(&MockSSD);

    for (auto testcase : VALID_READ_ARGV_LIST) {
        char** argv = const_cast<char**>(testcase.data());
        EXPECT_NO_THROW(host->Execute(VALID_READ_ARGC, argv));
    }
}

TEST_F(HostInterfaceFixture, WRITE_INVALID_ARGV)
{
    HostInterface* host = HostInterface::GetInstance();
    host->SetSSD(&MockSSD);

    for (auto testcase : INVALID_WRITE_ARGV_LIST) {
        char** argv = const_cast<char**>(testcase.data());
        EXPECT_THROW(host->Execute(VALID_WRITE_ARGC, argv), std::exception);
    }
}

TEST_F(HostInterfaceFixture, WRITE_VALID_ARGV)
{
    HostInterface* host = HostInterface::GetInstance();
    host->SetSSD(&MockSSD);

    for (auto testcase : VALID_WRITE_ARGV_LIST) {
        char** argv = const_cast<char**>(testcase.data());
        EXPECT_NO_THROW(host->Execute(VALID_WRITE_ARGC, argv), std::exception);
    }
}

#endif