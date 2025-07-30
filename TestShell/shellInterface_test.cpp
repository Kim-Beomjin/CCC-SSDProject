#ifdef _DEBUG
#include <iostream>
#include <string>
#include "gmock/gmock.h"
#include "shell.cpp"

using namespace std;
using namespace testing;

class ShellFixture : public Test {
public:
    class Shell shell;
};

TEST_F(ShellFixture, ExitCommand) {
    string fakeCmd = "exit";
    string EXIT_RESULT = "Shell> exit\n";
    istringstream iss(fakeCmd);
    ostringstream oss;

    auto* oldInputBuf = cin.rdbuf();
    streambuf* oldOutputBuf = cout.rdbuf();

    std::cin.rdbuf(iss.rdbuf());
    std::cout.rdbuf(oss.rdbuf());

    shell.RunShellLoop();

    std::cin.rdbuf(oldInputBuf);
    std::cout.rdbuf(oldOutputBuf);

    std::string result = oss.str();
    //std::cout << "output" << result;
    EXPECT_EQ(EXIT_RESULT, result);
}

TEST_F(ShellFixture, WriteAndExit) {
    string fakeCmd = "write 3 0x12345678\nexit";
    string WRITEANDEXIT_RESULT = "Shell> write 3 0x12345678\nShell> exit\n";
    istringstream iss(fakeCmd);
    ostringstream oss;

    auto* oldInputBuf = cin.rdbuf();
    streambuf* oldOutputBuf = cout.rdbuf();

    std::cin.rdbuf(iss.rdbuf());
    std::cout.rdbuf(oss.rdbuf());

    shell.RunShellLoop();

    std::cin.rdbuf(oldInputBuf);
    std::cout.rdbuf(oldOutputBuf);

    std::string result = oss.str();
    //std::cout << "output" << result;
    EXPECT_EQ(WRITEANDEXIT_RESULT, result);
}

TEST_F(ShellFixture, ReadAndExit) {
    string fakeCmd = "read 3\nexit";
    string READANDEXIT_RESULT = "Shell> read 3\nShell> exit\n";
    istringstream iss(fakeCmd);
    ostringstream oss;

    auto* oldInputBuf = cin.rdbuf();
    streambuf* oldOutputBuf = cout.rdbuf();

    std::cin.rdbuf(iss.rdbuf());
    std::cout.rdbuf(oss.rdbuf());

    shell.RunShellLoop();

    std::cin.rdbuf(oldInputBuf);
    std::cout.rdbuf(oldOutputBuf);

    std::string result = oss.str();
    //std::cout << "output" << result;
    EXPECT_EQ(READANDEXIT_RESULT, result);
}

#endif