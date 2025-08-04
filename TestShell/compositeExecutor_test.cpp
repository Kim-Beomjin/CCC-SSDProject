#ifdef _DEBUG
#include "gmock/gmock.h"
#include "compositeExecutor.h"

using namespace testing;
using std::shared_ptr;

class MockSsdApp : public ISsdApp {
public:
	MOCK_METHOD(bool, Read, (LBA), (override));
	MOCK_METHOD(bool, Write, (LBA, DATA), (override));
	MOCK_METHOD(bool, Erase, (LBA, SIZE), (override));
	MOCK_METHOD(bool, Flush, (), (override));
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

class MockEraser : public Eraser {
public:
	MOCK_METHOD(bool, execute, (ISsdApp*, LBA, SIZE), (override));
};

class CompositeExecutorFixture : public Test {
public:
	void SetUp() override {
		mockApp = make_shared<CompositeExecutor>(mockWriter, mockComparer, mockEraser);
	}

	const string BLANK_TEST_SCRIPT_NAME = "";
	const string INVALID_TEST_SCRIPT_NAME = "123";

	const int FIRST_TEST_SCRIPT_MAX_IO_TIMES =
		FullWriteReadCompareStrategy::LOOP_COUNT * FullWriteReadCompareStrategy::NUM_LBA_PER_LOOP;

	const int SECOND_TEST_SCRIPT_MAX_IO_TIMES =
		PartialLBAWriteStrategy::LOOP_COUNT * PartialLBAWriteStrategy::NUM_LBA_PER_LOOP;

	const int THIRD_TEST_SCRIPT_MAX_IO_TIMES =
		WriteReadAgingStrategy::LOOP_COUNT * WriteReadAgingStrategy::NUM_LBA_PER_LOOP;

	shared_ptr<CompositeExecutor> mockApp;

	shared_ptr<MockWriter> mockWriter = make_shared<NiceMock<MockWriter>>();
	shared_ptr<MockComparer> mockComparer = make_shared<NiceMock<MockComparer>>();
	shared_ptr<MockEraser> mockEraser = make_shared<NiceMock<MockEraser>>();
	NiceMock<MockSsdApp> mockSsdApp;
};

#if (FIX_ME_LATER == 0)
TEST_F(CompositeExecutorFixture, ThrowInvalidCompositeExecutor) {
	EXPECT_THROW(mockFirstApp.execute(BLANK_TEST_SCRIPT_NAME, &mockSsdApp), runtime_error);
	EXPECT_THROW(mockSecondApp.execute(BLANK_TEST_SCRIPT_NAME, &mockSsdApp), runtime_error);
	EXPECT_THROW(mockThirdApp.execute(BLANK_TEST_SCRIPT_NAME, &mockSsdApp), runtime_error);

	EXPECT_THROW(mockFirstApp.execute(INVALID_TEST_SCRIPT_NAME, &mockSsdApp), runtime_error);
	EXPECT_THROW(mockSecondApp.execute(INVALID_TEST_SCRIPT_NAME, &mockSsdApp), runtime_error);
	EXPECT_THROW(mockThirdApp.execute(INVALID_TEST_SCRIPT_NAME, &mockSsdApp), runtime_error);
}
#endif


TEST_F(CompositeExecutorFixture, CompositeExecutor1CheckMockReadWriteMaxTimes) {
	EXPECT_CALL(*mockWriter, execute)
		.Times(FIRST_TEST_SCRIPT_MAX_IO_TIMES);

	EXPECT_CALL(*mockComparer, execute)
		.Times(FIRST_TEST_SCRIPT_MAX_IO_TIMES)
		.WillRepeatedly(Return(true));

	shared_ptr<ICompositeExecutorStrategy> strategy = make_shared< FullWriteReadCompareStrategy>();
	mockApp->setStrategy(strategy);

	mockApp->execute(&mockSsdApp, 0, 0);
}

TEST_F(CompositeExecutorFixture, CompositeExecutor2CheckMockReadWriteMaxTimes) {
	EXPECT_CALL(*mockWriter, execute)
		.Times(SECOND_TEST_SCRIPT_MAX_IO_TIMES);

	EXPECT_CALL(*mockComparer, execute)
		.Times(SECOND_TEST_SCRIPT_MAX_IO_TIMES)
		.WillRepeatedly(Return(true));

	shared_ptr<ICompositeExecutorStrategy> strategy = make_shared< PartialLBAWriteStrategy>();
	mockApp->setStrategy(strategy);

	mockApp->execute(&mockSsdApp, 0, 0);
}

TEST_F(CompositeExecutorFixture, CompositeExecutor3CheckMockReadWriteMaxTimes) {
	EXPECT_CALL(*mockWriter, execute)
		.Times(THIRD_TEST_SCRIPT_MAX_IO_TIMES);

	EXPECT_CALL(*mockComparer, execute)
		.Times(THIRD_TEST_SCRIPT_MAX_IO_TIMES)
		.WillRepeatedly(Return(true));

	shared_ptr<ICompositeExecutorStrategy> strategy = make_shared< WriteReadAgingStrategy>();
	mockApp->setStrategy(strategy);

	mockApp->execute(&mockSsdApp, 0, 0);
}

#endif
