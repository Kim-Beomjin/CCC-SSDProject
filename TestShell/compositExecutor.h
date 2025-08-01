#pragma once
#include "global_config.h"
#include "executor.h"
#include <string>

using namespace std;

class CompositExecutor : public IExecutor
{
public:
	CompositExecutor() = default;
	CompositExecutor(Writer* writer, Comparer* comparer) : writer{ writer }, comparer { comparer } {}
	bool IsValidCommand(const vector<string>& tokens) override { return true; }
protected:
	static bool PrintPass(void);
	static bool PrintFail(void);

	Writer* writer;
	Comparer* comparer;
};

class FullWriteAndReadCompare : public CompositExecutor {
public:
	FullWriteAndReadCompare() = default;
	FullWriteAndReadCompare(Writer* writer, Comparer* comparer) : CompositExecutor{ writer, comparer } {}

	static const int LOOP_COUNT = 20;
	static const int NUM_LBA_PER_LOOP = 5;

	bool execute(ISsdApp* app, LBA lba, DATA data) override;
};

class PartialLBAWrite : public CompositExecutor {
public:
	PartialLBAWrite() = default;
	PartialLBAWrite(Writer* writer, Comparer* comparer) : CompositExecutor{ writer, comparer } {}

	static const int LOOP_COUNT = 30;
	static const int NUM_LBA_PER_LOOP = 5;

	bool execute(ISsdApp* app, LBA lba, DATA data) override;
};

class WriteReadAging : public CompositExecutor {
public:
	WriteReadAging() = default;
	WriteReadAging(Writer* writer, Comparer* comparer) : CompositExecutor{ writer, comparer } {}

	static const int LOOP_COUNT = 200;
	static const int NUM_LBA_PER_LOOP = 2;

	bool execute(ISsdApp* app, LBA lba, DATA data) override;
};

class EraseAndWriteAging : public CompositExecutor {
public:
	EraseAndWriteAging() = default;
	EraseAndWriteAging(Writer* writer, Comparer* comparer, Eraser* eraser) :
		CompositExecutor{ writer, comparer }, eraser { eraser } {}

	static const int LOOP_COUNT = 30;
	static const int NUM_LBA_PER_LOOP = 3;

	bool execute(ISsdApp* app, LBA lba, DATA data) override;

private:
	Eraser* eraser;
};

