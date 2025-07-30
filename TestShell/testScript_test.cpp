#ifdef _DEBUG
#include <string>
#include "gmock/gmock.h"
#include "testScript.h"

using namespace std;
using namespace testing;

class TestScriptFixture : public Test {
public:
    TestScript app;
};

TEST_F(TestScriptFixture, ThrowBlankTestScript) {
	string blankString = "";

	try {
		app.FullWriteAndReadCompare(blankString);
		FAIL();
	}
	catch (runtime_error& e) {
		EXPECT_EQ(string{ e.what() },
			string{ "Invalid Test Script" });
	}
}

TEST_F(TestScriptFixture, ThrowInvalidTestScript) {
	string invalidString = "123";

	try {
		app.FullWriteAndReadCompare(invalidString);
		FAIL();
	}
	catch (runtime_error& e) {
		EXPECT_EQ(string{ e.what() },
			string{ "Invalid Test Script" });
	}
}

#endif
