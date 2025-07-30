#ifdef _DEBUG
#include <iostream>
#include <string>
#include "gmock/gmock.h"
#include "shell.cpp"

using namespace std;
using namespace testing;

class MockCommandParser : public ICommandParserBridge {
public:
    MOCK_METHOD(bool, ParseCommand, (string cmd));
    MOCK_METHOD(bool, DoExecution, (string cmd));
};

class ShellFixture : public Test {
public:
    NiceMock<MockCommandParser> mockCommandParser;
    class Shell shell;
    class Shell mockshell {&mockCommandParser};
    string EXIT_CMD = "exit\n";
    string WRITE_CMD = "write 3 0x12345678\n";
    string READ_CMD = "read 3\n";
    string HELP_CMD = "help\n";

    string EXIT_RESULT = "Shell> exit\n";
    string WRITE_RESULT = "Shell> write 3 0x12345678\n";
    string READ_RESULT = "Shell> read 3\n";
    string HELP_RESULT = "Shell> help\n";

    const string HELP_DESCRIPTION =
        "팀명: CCC(Clean Code Collective) \n"
        "팀장 : 김범진 / 팀원 : 김경민, 김윤진, 김율곤, 정지윤\n\n"
        "Command 설명:\n"
        "write: 필요 파라미터 lba(0~99 중), data(32bit 16진수 값) - ssd의 lba cell에 data를 저장한다.\n"
        "fullwrite: 필요 파라미터 data(32bit 16진수 값) - ssd의 모든 cell에 data를 저장한다.\n"
        "read: 필요 파라미터 lba(0~99 중) - ssd의 lba cell의 값을 읽어온다.\n"
        "fullread: 필요 파라미터 NONE - ssd의 모든 cell의 값을 읽어온다\n"
        "help: 필요 파라미터 NONE - ssd에 필요한 command 등 모든 정보에 대한 도움말을 요청한다\n"
        "exit: 필요 파라미터 NONE - 하려던 거 끝내고 바로 종료한다\n";
    string HELPANDEXIT_RESULT = HELP_RESULT + HELP_DESCRIPTION + EXIT_RESULT;

    string GetResultFromRunShellLoop(class Shell *shell, string cmd) {
        istringstream iss(cmd);
        ostringstream oss;

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

    EXPECT_EQ(EXIT_RESULT, result);
}

TEST_F(ShellFixture, HelpAndExit) {
    string fakeCmd = HELP_CMD + EXIT_CMD;

    string result = GetResultFromRunShellLoop(&shell, fakeCmd);

    EXPECT_EQ(HELPANDEXIT_RESULT, result);
}

TEST_F(ShellFixture, WriteAndExit) {
    string fakeCmd = WRITE_CMD + EXIT_CMD;
    string WRITEANDEXIT_RESULT = WRITE_RESULT + EXIT_RESULT;

    string result = GetResultFromRunShellLoop(&shell, fakeCmd);

    EXPECT_EQ(WRITEANDEXIT_RESULT, result);
}

TEST_F(ShellFixture, ReadAndExit) {
    string fakeCmd = READ_CMD + EXIT_CMD;
    string READANDEXIT_RESULT = READ_RESULT + EXIT_RESULT;

    string result = GetResultFromRunShellLoop(&shell, fakeCmd);

    EXPECT_EQ(READANDEXIT_RESULT, result);
}

TEST_F(ShellFixture, MockExitCommand) {
    string fakeCmd = EXIT_CMD;

    EXPECT_CALL(mockCommandParser, ParseCommand)
        .Times(1)
        .WillOnce(Return(true));

    EXPECT_CALL(mockCommandParser, DoExecution)
        .Times(1)
        .WillRepeatedly(Return(true));

    string result = GetResultFromRunShellLoop(&mockshell, fakeCmd);
    EXPECT_EQ(EXIT_RESULT, result);
}

TEST_F(ShellFixture, MockWriteAndExit) {
    string fakeCmd = WRITE_CMD + EXIT_CMD;
    string WRITEANDEXIT_RESULT = WRITE_RESULT + EXIT_RESULT;

    EXPECT_CALL(mockCommandParser, ParseCommand)
        .Times(2)
        .WillRepeatedly(Return(true));

    EXPECT_CALL(mockCommandParser, DoExecution)
        .Times(2)
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true));

    string result = GetResultFromRunShellLoop(&mockshell, fakeCmd);

    EXPECT_EQ(WRITEANDEXIT_RESULT, result);
}

TEST_F(ShellFixture, MockReadAndExit) {
    string fakeCmd = READ_CMD + EXIT_CMD;
    string READANDEXIT_RESULT = READ_RESULT + EXIT_RESULT;

    EXPECT_CALL(mockCommandParser, ParseCommand)
        .Times(2)
        .WillRepeatedly(Return(true));

    EXPECT_CALL(mockCommandParser, DoExecution)
        .Times(2)
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true));

    string result = GetResultFromRunShellLoop(&mockshell, fakeCmd);

    EXPECT_EQ(READANDEXIT_RESULT, result);
}

TEST_F(ShellFixture, MockHelpAndExit) {
    string fakeCmd = HELP_CMD + EXIT_CMD;
    string READANDEXIT_RESULT = HELP_RESULT + EXIT_RESULT;

    EXPECT_CALL(mockCommandParser, ParseCommand)
        .Times(2)
        .WillRepeatedly(Return(true));

    EXPECT_CALL(mockCommandParser, DoExecution)
        .Times(2)
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true));

    string result = GetResultFromRunShellLoop(&mockshell, fakeCmd);

    EXPECT_EQ(READANDEXIT_RESULT, result);
}

#endif
