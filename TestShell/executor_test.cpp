#ifdef _DEBUG
#include <string>
#include "gmock/gmock.h"
#include "global_config.h"
#include "interface.h"
#include "executorFactory.h"

using namespace testing;

class MockSsdApp : public ISsdApp {
public:
	MOCK_METHOD(bool, Read, (LBA), (override));
	MOCK_METHOD(bool, Write, (LBA, DATA), (override));
	MOCK_METHOD(bool, Erase, (LBA, SIZE), (override));
	MOCK_METHOD(bool, Flush, (), (override));
};

class ExecutorTestFixture : public Test {
public:
	void SetUp() override {
		factory = new ExecutorFactory();
	}

	void TearDown() override {
		delete factory;
	}

	void checkExecute(string cmd = "", LBA lba = 0, DATA data = 0, DATA expect_data = 0) {
		executor = factory->createExecutor(cmd);
		EXPECT_TRUE(executor->execute(&mock_app, lba, data));
	}

	IExecutorFactory* factory;
	MockSsdApp mock_app;
	shared_ptr<IExecutor> executor;
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

TEST_F(ExecutorTestFixture, readNonWriteTest) {
	EXPECT_CALL(mock_app, Read)
		.Times(1)
		.WillOnce(Return(NO_DATA));

	checkExecute(READ_CMD, TEST_LBA, 0, NO_DATA);
}
#ifdef _FULL_TEST_RUN
TEST_F(ExecutorTestFixture, fullWriteCommandTest) {
	EXPECT_CALL(mock_app, Write)
		.Times(SSD_MAX_SIZE)
		.WillRepeatedly(Return(true));

	checkExecute(FULL_WRITE_CMD, 0, TEST_DATA);
}

TEST_F(ExecutorTestFixture, fullreadCommandTest) {
	EXPECT_CALL(mock_app, Read)
		.Times(SSD_MAX_SIZE)
		.WillRepeatedly(Return(NO_DATA));

	checkExecute(FULL_READ_CMD, 0, 0, NO_DATA);
}
#endif

TEST_F(ExecutorTestFixture, eraseCommandTest) {
	EXPECT_CALL(mock_app, Erase)
		.Times(1)
		.WillOnce(Return(true));

	checkExecute(ERASE_CMD, 0, 2);
}

TEST_F(ExecutorTestFixture, eraseRangeCommandTest) {
	EXPECT_CALL(mock_app, Erase)
		.Times(1)
		.WillOnce(Return(true));

	checkExecute(ERASE_RANGE_CMD, 3, 10);
}

TEST_F(ExecutorTestFixture, flushCommandTest) {
	EXPECT_CALL(mock_app, Flush)
		.Times(1)
		.WillOnce(Return(true));

	checkExecute(FLUSH_CMD);
}

#endif