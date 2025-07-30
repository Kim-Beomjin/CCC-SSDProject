#ifdef _DEBUG
#include <string>
#include "gmock/gmock.h"
#include "interface.h"

using namespace testing;

class MockSsdApp : public ISsdApp {
public:
	MOCK_METHOD(string, Read, (int), (override));
	MOCK_METHOD(bool, Write, (int, const string&), (override));
};

class ExecutorTestFixture : public Test {
public:
	void SetUpExecutor(string cmd) {
		//static ISsdApp& fake_app = SsdAppFactory().GetApp("fake");
		executor = ExecutorFactory().createExecutor(cmd, &mock_app);
	}

	IExecutor* executor;
	MockSsdApp mock_app;

	const string READ_CMD = "read";
	const string WRITE_CMD = "write";
	const string FULL_READ_CMD = "fullread";
	const string FULL_WRITE_CMD = "fullwrite";
	const string HELP_CMD = "help";
	const string EXIT_CMD = "exit";

	const int TEST_LBA = 2;
	const string TEST_DATA = "0x12345678";
	const string NO_DATA = "0x00000000";
};

TEST_F(ExecutorTestFixture, exitCommandTest) {
	SetUpExecutor(EXIT_CMD);

	bool ret = executor->execute();
	EXPECT_TRUE(ret);
}

TEST_F(ExecutorTestFixture, helpCommandTest) {
	SetUpExecutor(HELP_CMD);

	EXPECT_TRUE(executor->execute());
}

TEST_F(ExecutorTestFixture, writeCommandTest) {
	SetUpExecutor(WRITE_CMD);

	EXPECT_CALL(mock_app, Write)
		.Times(1)
		.WillOnce(Return(true));

	EXPECT_TRUE(executor->execute(WRITE_CMD, TEST_LBA, TEST_DATA));
}

TEST_F(ExecutorTestFixture, fullWriteCommandTest) {
	SetUpExecutor(WRITE_CMD);

	EXPECT_CALL(mock_app, Write)
		.Times(100)
		.WillRepeatedly(Return(true));

	EXPECT_TRUE(executor->execute(FULL_WRITE_CMD, TEST_LBA));
}

TEST_F(ExecutorTestFixture, readNonWriteTest) {
	SetUpExecutor(READ_CMD);

	EXPECT_CALL(mock_app, Read)
		.Times(1)
		.WillOnce(Return(NO_DATA));

	EXPECT_TRUE(executor->execute(READ_CMD, TEST_LBA));
}

TEST_F(ExecutorTestFixture, fullreadCommandTest) {
	SetUpExecutor(READ_CMD);

	EXPECT_CALL(mock_app, Read)
		.Times(100)
		.WillRepeatedly(Return(NO_DATA));

	EXPECT_TRUE(executor->execute(FULL_READ_CMD));
}

#endif