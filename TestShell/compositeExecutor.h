#pragma once
#include "global_config.h"
#include "executor.h"
#include <string>

using namespace std;

class CompositeExecutor : public IExecutor
{
public:
	CompositeExecutor() = default;
	CompositeExecutor(Writer* writer, Comparer* comparer) : writer{ writer }, comparer { comparer } {}

protected:
	static bool PrintPass(void);
	static bool PrintFail(void);

	Writer* writer;
	Comparer* comparer;
};

class FullWriteAndReadCompare : public CompositeExecutor {
public:
	FullWriteAndReadCompare() = default;
	FullWriteAndReadCompare(Writer* writer, Comparer* comparer) : CompositeExecutor{ writer, comparer } {}
	bool IsValidCommand(const vector<string>& tokens) override;

	static const int LOOP_COUNT = 20;
	static const int NUM_LBA_PER_LOOP = 5;

	bool execute(ISsdApp* app, LBA lba, DATA data) override;
};

class PartialLBAWrite : public CompositeExecutor {
public:
	PartialLBAWrite() = default;
	PartialLBAWrite(Writer* writer, Comparer* comparer) : CompositeExecutor{ writer, comparer } {}
	bool IsValidCommand(const vector<string>& tokens) override;

	static const int LOOP_COUNT = 30;
	static const int NUM_LBA_PER_LOOP = 5;

	bool execute(ISsdApp* app, LBA lba, DATA data) override;
};

class WriteReadAging : public CompositeExecutor {
public:
	WriteReadAging() = default;
	WriteReadAging(Writer* writer, Comparer* comparer) : CompositeExecutor{ writer, comparer } {}
	bool IsValidCommand(const vector<string>& tokens) override;

	static const int LOOP_COUNT = 200;
	static const int NUM_LBA_PER_LOOP = 2;

	bool execute(ISsdApp* app, LBA lba, DATA data) override;
};

class EraseAndWriteAging : public CompositeExecutor {
public:
	EraseAndWriteAging() = default;
	EraseAndWriteAging(Writer* writer, Comparer* comparer, Eraser* eraser) :
		CompositeExecutor{ writer, comparer }, eraser { eraser } {}
	bool IsValidCommand(const vector<string>& tokens) override;

	static const int LOOP_COUNT = 30;
	static const int NUM_LBA_PER_LOOP = 3;

	bool execute(ISsdApp* app, LBA lba, DATA data) override;

private:
	Eraser* eraser;
};

