#pragma once
#include "global_config.h"
#include "executor.h"
#include <string>

using namespace std;
using std::shared_ptr;

struct ICompositeExecutorStrategy
{
    virtual bool run(ISsdApp* app, shared_ptr<Writer> writer, shared_ptr<Comparer> comparer, shared_ptr<Eraser> eraser) = 0;
};

class CompositeExecutor : public IExecutor
{
public:
	CompositeExecutor()
	{
		writer = std::make_shared<Writer>();
		comparer = std::make_shared<Comparer>();
		eraser = std::make_shared<Eraser>();
	}
	CompositeExecutor(shared_ptr<Writer> writer, shared_ptr<Comparer> comparer, shared_ptr<Eraser> eraser) :
		writer{ writer }, comparer{ comparer }, eraser{ eraser } {}

	bool IsValidCommand(const vector<string>& tokens) override;

	bool execute(ISsdApp* app, LBA lba, DATA data) override;

	void setStrategy(shared_ptr<ICompositeExecutorStrategy> newStrategy) {
		strategy = newStrategy;
	}

private:
	bool PrintPass(void);
	bool PrintFail(void);

	shared_ptr<ICompositeExecutorStrategy> strategy;
	shared_ptr<Writer> writer;
	shared_ptr<Comparer> comparer;
	shared_ptr<Eraser> eraser;
};

class FullWriteReadCompareStrategy : public ICompositeExecutorStrategy {
public:
	static const int LOOP_COUNT = 20;
	static const int NUM_LBA_PER_LOOP = 5;

    bool run(ISsdApp* app, shared_ptr<Writer> writer, shared_ptr<Comparer> comparer, shared_ptr<Eraser> eraser) override;
};

class PartialLBAWriteStrategy : public ICompositeExecutorStrategy {
public:
	static const int LOOP_COUNT = 30;
	static const int NUM_LBA_PER_LOOP = 5;

    bool run(ISsdApp* app, shared_ptr<Writer> writer, shared_ptr<Comparer> comparer, shared_ptr<Eraser> eraser) override;
};

class WriteReadAgingStrategy : public ICompositeExecutorStrategy {
public:
	static const int LOOP_COUNT = 200;
	static const int NUM_LBA_PER_LOOP = 2;

    bool run(ISsdApp* app, shared_ptr<Writer> writer, shared_ptr<Comparer> comparer, shared_ptr<Eraser> eraser) override;
};

class EraseWriteAgingStrategy : public ICompositeExecutorStrategy {
public:
	static const int LOOP_COUNT = 30;
	static const int NUM_LBA_PER_LOOP = 3;

    bool run(ISsdApp* app, shared_ptr<Writer> writer, shared_ptr<Comparer> comparer, shared_ptr<Eraser> eraser) override;
};

