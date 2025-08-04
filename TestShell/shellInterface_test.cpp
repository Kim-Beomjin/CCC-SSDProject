#ifdef _DEBUG
#include <iostream>
#include <string>
#include "gmock/gmock.h"
#include "shell.h"
#include "global_config.h"
#include "interface.h"

using namespace std;
using namespace testing;

class MockCommandParser : public ICommandParserBridge {
public:
    MOCK_METHOD(bool, ParseCommand, (const string&), (override));
    MOCK_METHOD(bool, ExecuteSsdUsingParsedCommand, (ISsdApp*), (override));
};

class MockSsdApp : public ISsdApp {
public:
    MOCK_METHOD(bool, Read, (LBA), (override));
    MOCK_METHOD(bool, Write, (LBA, DATA), (override));
    MOCK_METHOD(bool, Erase, (LBA, SIZE), (override));
    MOCK_METHOD(bool, Flush, (), (override));
};

class ShellFixture : public Test {
public:
    shared_ptr<MockCommandParser> mockCommandParser = make_shared<NiceMock<MockCommandParser>>();
    NiceMock<MockSsdApp> mock_app;
    class Shell shell;
    class Shell mockshell { mockCommandParser };
    string EXIT_CMD = "exit\n";
    string WRITE_CMD = "write 3 0x12345678\n";
    string READ_CMD = "read 3\n";

    string EXIT_RESULT = "Shell> exit\n";
    string WRITE_RESULT = "Shell> write 3 0x12345678\n";
    string READ_RESULT = "Shell> read 3\n";
    string WRITE_DONE = "[Write] Done\n";
    string READ_DONE = "[Read] LBA: 3 : 0x00000000\n";

    string GetResultFromRunShellLoop(class Shell *shell, string cmd) {
        shell->setSsdApp(&mock_app);

        istringstream iss(cmd);
        ostringstream oss = {};

        auto* oldInputBuf = cin.rdbuf();
        streambuf* oldOutputBuf = cout.rdbuf();

        cin.rdbuf(iss.rdbuf());
        cout.rdbuf(oss.rdbuf());

        shell->RunShellLoop();

        cin.rdbuf(oldInputBuf);
        cout.rdbuf(oldOutputBuf);

        return oss.str();
    }
};

TEST_F(ShellFixture, ExitCommand) {
    string fakeCmd = EXIT_CMD;

    string result = GetResultFromRunShellLoop(&shell, fakeCmd);
    //std::cout << "output" << result;
    EXPECT_EQ(EXIT_RESULT, result);
}

TEST_F(ShellFixture, WriteAndExit) {
    string fakeCmd = WRITE_CMD + EXIT_CMD;
    string WRITEANDEXIT_RESULT = WRITE_RESULT + WRITE_DONE + EXIT_RESULT;

    EXPECT_CALL(mock_app, Write)
        .Times(1)
        .WillOnce(Return(true));

    string result = GetResultFromRunShellLoop(&shell, fakeCmd);

    EXPECT_EQ(WRITEANDEXIT_RESULT, result);
}

TEST_F(ShellFixture, ReadAndExit) {
    string fakeCmd = READ_CMD + EXIT_CMD;
    string READANDEXIT_RESULT = READ_RESULT + READ_DONE + EXIT_RESULT;

    EXPECT_CALL(mock_app, Read)
        .Times(1)
        .WillOnce(Return(true));

    string result = GetResultFromRunShellLoop(&shell, fakeCmd);

    EXPECT_EQ(READANDEXIT_RESULT, result);
}

TEST_F(ShellFixture, MockExitCommand) {
    string fakeCmd = EXIT_CMD;

    EXPECT_CALL(*mockCommandParser, ParseCommand)
        .Times(1)
        .WillOnce(Return(true));

    string result = GetResultFromRunShellLoop(&mockshell, fakeCmd);
    EXPECT_EQ(EXIT_RESULT, result);
}

TEST_F(ShellFixture, MockWriteAndExit) {
    string fakeCmd = WRITE_CMD + EXIT_CMD;
    string WRITEANDEXIT_RESULT = WRITE_RESULT + EXIT_RESULT;

    EXPECT_CALL(*mockCommandParser, ParseCommand)
        .Times(2)
        .WillRepeatedly(Return(true));

    string result = GetResultFromRunShellLoop(&mockshell, fakeCmd);

    EXPECT_EQ(WRITEANDEXIT_RESULT, result);
}

TEST_F(ShellFixture, MockReadAndExit) {
    string fakeCmd = READ_CMD + EXIT_CMD;
    string READANDEXIT_RESULT = READ_RESULT + EXIT_RESULT;

    EXPECT_CALL(*mockCommandParser, ParseCommand)
        .Times(2)
        .WillRepeatedly(Return(true));

    string result = GetResultFromRunShellLoop(&mockshell, fakeCmd);

    EXPECT_EQ(READANDEXIT_RESULT, result);
}

#endif
