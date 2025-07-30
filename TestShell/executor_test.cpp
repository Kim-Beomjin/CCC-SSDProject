#ifdef _DEBUG
#include <string>
#include "gmock/gmock.h"
#include "interface.h"

using namespace testing;

class MockSsdApp : public ISsdApp {
public:
	MOCK_METHOD(u32, Read, (u32), (override));
	MOCK_METHOD(bool, Write, (u32, u32), (override));
};

class ExecutorTestFixture : public Test {
public:
	void checkExecute(string cmd = "", u32 lba = 0, u32 data = 0, u32 expect_data = 0) {
		executor = ExecutorFactory().createExecutor(cmd);

		bool ret = executor->execute(cmd, lba, data, &mock_app);
		EXPECT_TRUE(ret);
	}

	MockSsdApp mock_app;
	IExecutor* executor;

	const string READ_CMD = "read";
	const string WRITE_CMD = "write";
	const string FULL_READ_CMD = "fullread";
	const string FULL_WRITE_CMD = "fullwrite";
	const string HELP_CMD = "help";
	const string EXIT_CMD = "exit";

	const u32 TEST_LBA = 2;
	const u32 TEST_DATA = 0x12345678;
	const u32 NO_DATA = 0x00000000;
};

TEST_F(ExecutorTestFixture, exitCommandTest) {
	checkExecute(EXIT_CMD);
}

TEST_F(ExecutorTestFixture, helpCommandTest) {
	checkExecute(HELP_CMD);
}

TEST_F(ExecutorTestFixture, writeCommandTest) {
	EXPECT_CALL(mock_app, Write)
		.Times(1)
		.WillOnce(Return(true));

	checkExecute(WRITE_CMD, TEST_LBA, TEST_DATA);
}

TEST_F(ExecutorTestFixture, fullWriteCommandTest) {
	EXPECT_CALL(mock_app, Write)
		.Times(100)
		.WillRepeatedly(Return(true));
	
	checkExecute(FULL_WRITE_CMD, 0, TEST_DATA);
}

TEST_F(ExecutorTestFixture, readNonWriteTest) {
	EXPECT_CALL(mock_app, Read)
		.Times(1)
		.WillOnce(Return(NO_DATA));

	checkExecute(READ_CMD, TEST_LBA, 0, NO_DATA);
}

TEST_F(ExecutorTestFixture, fullreadCommandTest) {
	EXPECT_CALL(mock_app, Read)
		.Times(100)
		.WillRepeatedly(Return(NO_DATA));

	checkExecute(FULL_READ_CMD, 0, 0, NO_DATA);
}

#endif