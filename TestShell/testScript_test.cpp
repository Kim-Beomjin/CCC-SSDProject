#ifdef _DEBUG
#include <string>
#include "gmock/gmock.h"
#include "testScript.h"

using namespace std;
using namespace testing;

class TestScriptFixture : public Test {
public:
    TestScript app;

	const string BLANK_TEST_SCRIPT_NAME = "";
	const string INVALID_TEST_SCRIPT_NAME = "123";
};

TEST_F(TestScriptFixture, ThrowInvalidTestScript) {
	EXPECT_THROW(app.FullWriteAndReadCompare(BLANK_TEST_SCRIPT_NAME), runtime_error);
	EXPECT_THROW(app.FullWriteAndReadCompare(INVALID_TEST_SCRIPT_NAME), runtime_error);
}

#endif
