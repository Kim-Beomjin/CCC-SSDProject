#ifdef _DEBUG
#include "gmock/gmock.h"
#include "CompositExecutor.h"

using namespace testing;

class MockSsdApp : public ISsdApp {
public:
	MOCK_METHOD(DATA, Read, (LBA), (override));
	MOCK_METHOD(bool, Write, (LBA, DATA), (override));
	MOCK_METHOD(bool, Erase, (LBA, SIZE), (override));
};

class MockWriter : public Writer
{
public:
	MOCK_METHOD(bool, execute, (ISsdApp*, LBA, DATA), (override));
};

class MockComparer : public Comparer {
public:
	MOCK_METHOD(bool, execute, (ISsdApp*, LBA, DATA), (override));
};

class CompositExecutorFixture : public Test {
public:
	FullWriteAndReadCompare mockFirstApp{ &mockWriter, &mockComparer };
	PartialLBAWrite mockSecondApp{ &mockWriter, &mockComparer };
	WriteReadAging mockThirdApp{ &mockWriter, &mockComparer };

	const string BLANK_TEST_SCRIPT_NAME = "";
	const string INVALID_TEST_SCRIPT_NAME = "123";

	const int FIRST_TEST_SCRIPT_MAX_IO_TIMES =
		FullWriteAndReadCompare::LOOP_COUNT * FullWriteAndReadCompare::NUM_LBA_PER_LOOP;

	const int SECOND_TEST_SCRIPT_MAX_IO_TIMES =
		PartialLBAWrite::LOOP_COUNT * PartialLBAWrite::NUM_LBA_PER_LOOP;

	const int THIRD_TEST_SCRIPT_MAX_IO_TIMES =
		WriteReadAging::LOOP_COUNT * WriteReadAging::NUM_LBA_PER_LOOP;

	NiceMock<MockWriter> mockWriter;
	NiceMock<MockComparer> mockComparer;
	NiceMock<MockSsdApp> mockSsdApp;
};

#if (FIX_ME_LATER == 0)
TEST_F(CompositExecutorFixture, ThrowInvalidCompositExecutor) {
	EXPECT_THROW(mockFirstApp.execute(BLANK_TEST_SCRIPT_NAME, &mockSsdApp), runtime_error);
	EXPECT_THROW(mockSecondApp.execute(BLANK_TEST_SCRIPT_NAME, &mockSsdApp), runtime_error);
	EXPECT_THROW(mockThirdApp.execute(BLANK_TEST_SCRIPT_NAME, &mockSsdApp), runtime_error);

	EXPECT_THROW(mockFirstApp.execute(INVALID_TEST_SCRIPT_NAME, &mockSsdApp), runtime_error);
	EXPECT_THROW(mockSecondApp.execute(INVALID_TEST_SCRIPT_NAME, &mockSsdApp), runtime_error);
	EXPECT_THROW(mockThirdApp.execute(INVALID_TEST_SCRIPT_NAME, &mockSsdApp), runtime_error);
}
#endif

TEST_F(CompositExecutorFixture, CompositExecutor1CheckMockReadWriteMaxTimes) {
	EXPECT_CALL(mockWriter, execute)
		.Times(FIRST_TEST_SCRIPT_MAX_IO_TIMES);

	EXPECT_CALL(mockComparer, execute)
		.Times(FIRST_TEST_SCRIPT_MAX_IO_TIMES);

	mockFirstApp.execute(&mockSsdApp, 0, 0);
}

TEST_F(CompositExecutorFixture, CompositExecutor2CheckMockReadWriteMaxTimes) {
	EXPECT_CALL(mockWriter, execute)
		.Times(SECOND_TEST_SCRIPT_MAX_IO_TIMES);

	EXPECT_CALL(mockComparer, execute)
		.Times(SECOND_TEST_SCRIPT_MAX_IO_TIMES);

	mockSecondApp.execute(&mockSsdApp, 0, 0);
}

TEST_F(CompositExecutorFixture, CompositExecutor3CheckMockReadWriteMaxTimes) {
	EXPECT_CALL(mockWriter, execute)
		.Times(THIRD_TEST_SCRIPT_MAX_IO_TIMES);

	EXPECT_CALL(mockComparer, execute)
		.Times(THIRD_TEST_SCRIPT_MAX_IO_TIMES);

	mockThirdApp.execute(&mockSsdApp, 0, 0);
}

#endif
