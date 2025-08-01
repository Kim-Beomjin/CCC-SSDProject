#ifdef _DEBUG
#include "gmock/gmock.h"
#include "commandParser.h"
#include "utils.h"

using namespace testing;

class MockSsdApp : public ISsdApp {
public:
    MOCK_METHOD(DATA, Read, (LBA), (override));
    MOCK_METHOD(bool, Write, (LBA, DATA), (override));
    MOCK_METHOD(bool, Erase, (LBA, DATA), (override));
    MOCK_METHOD(bool, Flush, (), (override));
};

class CommandParserFixture : public Test {
public:
    class CommandParser parser;
    MockSsdApp mock_app;
    void CheckParseCommand(const string& cmd) {
        EXPECT_TRUE(parser.ParseCommand(cmd));
        EXPECT_TRUE(parser.ExecuteSsdUsingParsedCommand(&mock_app));
    }
};

TEST_F(CommandParserFixture, HelpCommand) {
    CheckParseCommand(HELP_CMD);
}
#ifdef _FULL_TEST_RUN
TEST_F(CommandParserFixture, FullwriteCommand) {
    EXPECT_CALL(mock_app, Write)
        .Times(SSD_MAX_SIZE)
        .WillRepeatedly(Return(true));

    CheckParseCommand(FULL_WRITE_CMD + " " + DataToHexString(TEST_DATA));
}

TEST_F(CommandParserFixture, FullreadCommand) {
    EXPECT_CALL(mock_app, Read)
        .Times(SSD_MAX_SIZE)
        .WillRepeatedly(Return(true));

    CheckParseCommand(FULL_READ_CMD);
}
#endif
TEST_F(CommandParserFixture, writeCommand) {
    EXPECT_CALL(mock_app, Write)
        .Times(1)
        .WillOnce(Return(true));

    CheckParseCommand(WRITE_CMD + " " + to_string(TEST_LBA) + " "  + DataToHexString(TEST_DATA));
}

TEST_F(CommandParserFixture, readCommand) {
    EXPECT_CALL(mock_app, Read)
        .Times(1)
        .WillOnce(Return(true));

    CheckParseCommand(READ_CMD + " " + to_string(TEST_LBA));
}

TEST_F(CommandParserFixture, eraseCommand) {
    EXPECT_CALL(mock_app, Erase)
        .Times(1)
        .WillOnce(Return(true));

    CheckParseCommand(ERASE_CMD + " " + to_string(TEST_START_LBA) + " " + to_string(TEST_SIZE));
}

TEST_F(CommandParserFixture, eraseWithMinusSizeCommand) {
    EXPECT_CALL(mock_app, Erase)
        .Times(2)
        .WillRepeatedly(Return(true));

    CheckParseCommand(ERASE_CMD + " " + to_string(TEST_START_LBA_VER_2) + " " + to_string(TEST_MINUS_SIZE));
}

TEST_F(CommandParserFixture, eraseRangeCommand) {
    EXPECT_CALL(mock_app, Erase)
        .Times(2)
        .WillRepeatedly(Return(true));

    CheckParseCommand(ERASE_RANGE_CMD + " " + to_string(TEST_START_LBA) + " " + to_string(TEST_END_LBA));
}

TEST_F(CommandParserFixture, eraseRangeReverseCaseCommand) {
    EXPECT_CALL(mock_app, Erase)
        .Times(2)
        .WillRepeatedly(Return(true));

    CheckParseCommand(ERASE_RANGE_CMD + " " + to_string(TEST_END_LBA) + " " + to_string(TEST_START_LBA));
}


TEST_F(CommandParserFixture, flushCommand) {
    EXPECT_CALL(mock_app, Flush)
        .Times(1)
        .WillOnce(Return(true));

    CheckParseCommand(FLUSH_CMD);
}

#endif
