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
    string cmd = "exit";

    shell.LoopGetCommand();

    //EXPECT_EQ(true, ret);
}

#endif