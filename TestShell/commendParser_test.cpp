#ifdef _DEBUG
#include <string>
#include "gmock/gmock.h"
#include "commandParser.cpp"

using namespace std;
using namespace testing;

class CommandParserFixture : public Test {
public:
    class CommandParser parser;
    int ret = true;
};

TEST_F(CommandParserFixture, HelpCommand) {
    string cmd = "help";

    ret = parser.ParseCommend(cmd);

    EXPECT_EQ(true, ret);
}

TEST_F(CommandParserFixture, FullwriteCommand) {
    string cmd = "fullwrite";

    ret = parser.ParseCommend(cmd);

    EXPECT_EQ(true, ret);
}

TEST_F(CommandParserFixture, fullreadCommand) {
    string cmd = "fullread";

    ret = parser.ParseCommend(cmd);

    EXPECT_EQ(true, ret);
}
#endif
