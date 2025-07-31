#ifdef _DEBUG
#include <string>
#include "gmock/gmock.h"
#include "global_config.h"
#include "interface.h"

using namespace testing;

class MockSsdApp : public ISsdApp {
public:
	MOCK_METHOD(DATA, Read, (LBA), (override));
	MOCK_METHOD(bool, Write, (LBA, DATA), (override));
	MOCK_METHOD(bool, Erase, (LBA, SIZE), (override));
	MOCK_METHOD(bool, Flush, (), (override));
};

class ExecutorTestFixture : public Test {
public:
	void checkExecute(string cmd = "", LBA lba = 0, DATA data = 0, DATA expect_data = 0) {
		executor = ExecutorFactory().createExecutor(cmd);

		bool ret = executor->execute(&mock_app, lba, data);
		EXPECT_TRUE(ret);
	}

	MockSsdApp mock_app;
	IExecutor* executor;
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

TEST_F(ExecutorTestFixture, fullwriteCommandTest) {
	EXPECT_CALL(mock_app, Write)
		.Times(SSD_MAX_SIZE)
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
		.Times(SSD_MAX_SIZE)
		.WillRepeatedly(Return(NO_DATA));

	checkExecute(FULL_READ_CMD, 0, 0, NO_DATA);
}

TEST_F(ExecutorTestFixture, eraseCommandTest) {
	EXPECT_CALL(mock_app, Erase)
		.Times(1)
		.WillOnce(Return(true));

	checkExecute(ERASE_CMD, 0, 2);
}

#endif