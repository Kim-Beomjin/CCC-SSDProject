#ifdef _DEBUG
#include "gmock/gmock.h"
#include "CompositExecutor.h"

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

class CompositExecutorFixture : public Test {
public:
	FullWriteAndReadCompare mockFirstApp{ &mockWriter, &mockReader };
	PartialLBAWrite mockSecondApp{ &mockWriter, &mockReader };
	WriteReadAging mockThirdApp{ &mockWriter, &mockReader };

	const string BLANK_TEST_SCRIPT_NAME = "";
	const string INVALID_TEST_SCRIPT_NAME = "123";
	const string FIRST_TEST_SCRIPT_NAME = "1_";
	const string SECOND_TEST_SCRIPT_NAME = "2_";
	const string THIRD_TEST_SCRIPT_NAME = "3_";

	const int FIRST_TEST_SCRIPT_MAX_IO_TIMES =
		FullWriteAndReadCompare::LOOP_COUNT * FullWriteAndReadCompare::NUM_LBA_PER_LOOP;

	const int SECOND_TEST_SCRIPT_MAX_IO_TIMES =
		PartialLBAWrite::LOOP_COUNT * PartialLBAWrite::NUM_LBA_PER_LOOP;

	const int THIRD_TEST_SCRIPT_MAX_IO_TIMES =
		WriteReadAging::LOOP_COUNT * WriteReadAging::NUM_LBA_PER_LOOP;

	NiceMock<MockWriter> mockWriter;
	NiceMock<MockReader> mockReader;
	NiceMock<MockSsdApp> mockSsdApp;
};

/*
TEST_F(CompositExecutorFixture, ThrowInvalidCompositExecutor) {
	EXPECT_THROW(mockFirstApp.execute(BLANK_TEST_SCRIPT_NAME, &mockSsdApp), runtime_error);
	EXPECT_THROW(mockSecondApp.execute(BLANK_TEST_SCRIPT_NAME, &mockSsdApp), runtime_error);
	EXPECT_THROW(mockThirdApp.execute(BLANK_TEST_SCRIPT_NAME, &mockSsdApp), runtime_error);

	EXPECT_THROW(mockFirstApp.execute(INVALID_TEST_SCRIPT_NAME, &mockSsdApp), runtime_error);
	EXPECT_THROW(mockSecondApp.execute(INVALID_TEST_SCRIPT_NAME, &mockSsdApp), runtime_error);
	EXPECT_THROW(mockThirdApp.execute(INVALID_TEST_SCRIPT_NAME, &mockSsdApp), runtime_error);
}
*/

TEST_F(CompositExecutorFixture, CompositExecutor1CheckMockReadWriteMaxTimes) {
	EXPECT_CALL(mockWriter, execute)
		.Times(FIRST_TEST_SCRIPT_MAX_IO_TIMES);

	EXPECT_CALL(mockReader, execute)
		.Times(FIRST_TEST_SCRIPT_MAX_IO_TIMES);

	mockFirstApp.execute(FIRST_TEST_SCRIPT_NAME, 0, 0, &mockSsdApp);
}

TEST_F(CompositExecutorFixture, CompositExecutor2CheckMockReadWriteMaxTimes) {
	EXPECT_CALL(mockWriter, execute)
		.Times(SECOND_TEST_SCRIPT_MAX_IO_TIMES);

	EXPECT_CALL(mockReader, execute)
		.Times(SECOND_TEST_SCRIPT_MAX_IO_TIMES);

	mockSecondApp.execute(SECOND_TEST_SCRIPT_NAME, 0, 0, &mockSsdApp);
}

TEST_F(CompositExecutorFixture, CompositExecutor3CheckMockReadWriteMaxTimes) {
	EXPECT_CALL(mockWriter, execute)
		.Times(THIRD_TEST_SCRIPT_MAX_IO_TIMES);

	EXPECT_CALL(mockReader, execute)
		.Times(THIRD_TEST_SCRIPT_MAX_IO_TIMES);

	mockThirdApp.execute(THIRD_TEST_SCRIPT_NAME, 0, 0, &mockSsdApp);
}

#endif
