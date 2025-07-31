#ifdef _DEBUG
#include "gmock/gmock.h"
#include "commandParser.h"

using namespace testing;

class MockSsdApp : public ISsdApp {
public:
    MOCK_METHOD(DATA, Read, (LBA), (override));
    MOCK_METHOD(bool, Write, (LBA, DATA), (override));
    MOCK_METHOD(bool, Erase, (LBA, SIZE), (override));
    MOCK_METHOD(bool, Flush, (), (override));
};

class CommandParserFixture : public Test {
public:
    class CommandParser parser;
    MockSsdApp mock_app;
    void CheckParseCommand(const string& cmd, LBA lba = 0, DATA data = 0) {
        EXPECT_TRUE(parser.ParseCommand(cmd));
        EXPECT_TRUE(parser.ExecuteSsdUsingParsedCommand(&mock_app));
    }
};

TEST_F(CommandParserFixture, HelpCommand) {
    CheckParseCommand(HELP_CMD);
}

TEST_F(CommandParserFixture, FullwriteCommand) {
    EXPECT_CALL(mock_app, Write)
        .Times(SSD_MAX_SIZE)
        .WillOnce(Return(true));

    CheckParseCommand(FULL_WRITE_CMD, 0, TEST_DATA);
}

TEST_F(CommandParserFixture, fullreadCommand) {
    EXPECT_CALL(mock_app, Read)
        .Times(SSD_MAX_SIZE)
        .WillOnce(Return(true));

    CheckParseCommand(FULL_READ_CMD);
}

TEST_F(CommandParserFixture, eraseCommand) {
    EXPECT_CALL(mock_app, Erase)
        .Times(1)
        .WillOnce(Return(true));

    CheckParseCommand(ERASE_CMD, 0, 2);
}

#endif
