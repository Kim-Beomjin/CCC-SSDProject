#ifdef _DEBUG
#include <string>
#include "gmock/gmock.h"
#include "commandParser.cpp"

using namespace testing;

class ExecutorTestFixture : public Test {
public:
	void SetUpExecutor(string cmd) {
		executor = ExecutorFactory().createExecutor(cmd);
	}

	IExecutor* executor;

	const string READ_CMD = "read";
	const string WRITE_CMD = "write";
	const string FULL_READ_CMD = "fullread";
	const string FULL_WRITE_CMD = "fullwrite";
	const string HELP_CMD = "help";
	const string EXIT_CMD = "exit";

	const int TEST_LBA = 2;
	const int TEST_DATA = 0x12345678;
};

TEST_F(ExecutorTestFixture, exitCommandTest) {
	SetUpExecutor(EXIT_CMD);

	EXPECT_TRUE(executor->execute());
}

TEST_F(ExecutorTestFixture, helpCommandTest) {
	SetUpExecutor(HELP_CMD);

	EXPECT_TRUE(executor->execute());
}

TEST_F(ExecutorTestFixture, writeCommandTest) {
	SetUpExecutor(WRITE_CMD);

	EXPECT_TRUE(executor->execute(WRITE_CMD, TEST_LBA, TEST_DATA));
}

TEST_F(ExecutorTestFixture, fullWriteCommandTest) {
	SetUpExecutor(WRITE_CMD);

	EXPECT_TRUE(executor->execute(FULL_WRITE_CMD, TEST_LBA));
}

TEST_F(ExecutorTestFixture, readAfterWriteTest) {
	SetUpExecutor(WRITE_CMD);
	EXPECT_TRUE(executor->execute(WRITE_CMD, TEST_LBA, TEST_DATA));

	SetUpExecutor(READ_CMD);
	EXPECT_TRUE(executor->execute(READ_CMD, TEST_LBA));
}

TEST_F(ExecutorTestFixture, readNonWriteTest) {
	SetUpExecutor(READ_CMD);
	EXPECT_TRUE(executor->execute(READ_CMD, TEST_LBA));
}

TEST_F(ExecutorTestFixture, fullreadCommandTest) {
	SetUpExecutor(READ_CMD);
	EXPECT_TRUE(executor->execute(FULL_READ_CMD));
}

#endif