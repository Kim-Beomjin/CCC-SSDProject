#ifdef _DEBUG
#include "gmock/gmock.h"
#include "testScript.h"

using namespace testing;

class MockWriter : public Writer
{
public:
	MOCK_METHOD(bool, execute, (const string&, LBA, DATA, ISsdApp*), (override));
};

class MockReader : public Reader {
public:
	MOCK_METHOD(bool, execute, (const string&, LBA, DATA, ISsdApp*), (override));
};

class TestScriptFixture : public Test {
public:
	TestScript app;
	TestScript mockApp{ &mockWriter, &mockReader };

	const string BLANK_TEST_SCRIPT_NAME = "";
	const string INVALID_TEST_SCRIPT_NAME = "123";
	const string FIRST_TEST_SCRIPT_NAME = "1_";

	NiceMock<MockWriter> mockWriter;
	NiceMock<MockReader> mockReader;
};

TEST_F(TestScriptFixture, ThrowInvalidTestScript) {
	EXPECT_THROW(app.FullWriteAndReadCompare(BLANK_TEST_SCRIPT_NAME), runtime_error);
	EXPECT_THROW(app.FullWriteAndReadCompare(INVALID_TEST_SCRIPT_NAME), runtime_error);
}

TEST_F(TestScriptFixture, TestScript1CheckMockReadWriteTimes) {
	EXPECT_CALL(mockWriter, execute)
		.Times(100);

	EXPECT_CALL(mockReader, execute)
		.Times(100);

	mockApp.FullWriteAndReadCompare(FIRST_TEST_SCRIPT_NAME);
}

#endif
