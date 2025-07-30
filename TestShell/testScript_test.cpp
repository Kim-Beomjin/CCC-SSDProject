#ifdef _DEBUG
#include "gmock/gmock.h"
#include "testScript.h"

using namespace testing;

class MockSsdApp : public ISsdApp {
public:
	MOCK_METHOD(DATA, Read, (LBA), (override));
	MOCK_METHOD(bool, Write, (LBA, DATA), (override));
};

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
	const string SECOND_TEST_SCRIPT_NAME = "2_";
	const string THIRD_TEST_SCRIPT_NAME = "3_";

	const int FIRST_TEST_SCRIPT_MAX_IO_TIMES =
		TestScript::FIRST_TEST_SCRIPT_LOOP_COUNT * TestScript::FIRST_TEST_SCRIPT_LOOP_LBA;

	const int SECOND_TEST_SCRIPT_MAX_IO_TIMES =
		TestScript::SECOND_TEST_SCRIPT_LOOP_COUNT * TestScript::SECOND_TEST_SCRIPT_LOOP_LBA;

	const int THIRD_TEST_SCRIPT_MAX_IO_TIMES =
		TestScript::THIRD_TEST_SCRIPT_LOOP_COUNT * TestScript::THIRD_TEST_SCRIPT_LOOP_LBA;

	NiceMock<MockWriter> mockWriter;
	NiceMock<MockReader> mockReader;
	NiceMock<MockSsdApp> mockSsdApp;
};

TEST_F(TestScriptFixture, ThrowInvalidTestScript) {
	EXPECT_THROW(app.FullWriteAndReadCompare(BLANK_TEST_SCRIPT_NAME, &mockSsdApp), runtime_error);
	EXPECT_THROW(app.PartialLBAWrite(BLANK_TEST_SCRIPT_NAME, &mockSsdApp), runtime_error);
	EXPECT_THROW(app.WriteReadAging(BLANK_TEST_SCRIPT_NAME, &mockSsdApp), runtime_error);

	EXPECT_THROW(app.FullWriteAndReadCompare(INVALID_TEST_SCRIPT_NAME, &mockSsdApp), runtime_error);
	EXPECT_THROW(app.PartialLBAWrite(INVALID_TEST_SCRIPT_NAME, &mockSsdApp), runtime_error);
	EXPECT_THROW(app.WriteReadAging(INVALID_TEST_SCRIPT_NAME, &mockSsdApp), runtime_error);
}

TEST_F(TestScriptFixture, TestScript1CheckMockReadWriteMaxTimes) {
	EXPECT_CALL(mockWriter, execute)
		.Times(FIRST_TEST_SCRIPT_MAX_IO_TIMES);

	EXPECT_CALL(mockReader, execute)
		.Times(FIRST_TEST_SCRIPT_MAX_IO_TIMES);

	mockApp.FullWriteAndReadCompare(FIRST_TEST_SCRIPT_NAME, &mockSsdApp);
}

TEST_F(TestScriptFixture, TestScript2CheckMockReadWriteMaxTimes) {
	EXPECT_CALL(mockWriter, execute)
		.Times(SECOND_TEST_SCRIPT_MAX_IO_TIMES);

	EXPECT_CALL(mockReader, execute)
		.Times(SECOND_TEST_SCRIPT_MAX_IO_TIMES);

	mockApp.PartialLBAWrite(SECOND_TEST_SCRIPT_NAME, &mockSsdApp);
}

TEST_F(TestScriptFixture, TestScript3CheckMockReadWriteMaxTimes) {
	EXPECT_CALL(mockWriter, execute)
		.Times(THIRD_TEST_SCRIPT_MAX_IO_TIMES);

	EXPECT_CALL(mockReader, execute)
		.Times(THIRD_TEST_SCRIPT_MAX_IO_TIMES);

	mockApp.WriteReadAging(THIRD_TEST_SCRIPT_NAME, &mockSsdApp);
}

#endif
