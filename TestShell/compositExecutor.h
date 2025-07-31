#pragma once
#include "global_config.h"
#include "executor.h"
#include <string>

using namespace std;

class CompositExecutor : public IExecutor
{
public:
	CompositExecutor() = default;
	CompositExecutor(Writer* writer, Reader* reader) : writer{ writer }, reader{ reader } {}
	bool ReadCompare(ISsdApp* app, LBA lba, DATA expectedData);

protected:
	Writer* writer;
	Reader* reader;
};

class FullWriteAndReadCompare : public CompositExecutor {
public:
	FullWriteAndReadCompare() = default;
	FullWriteAndReadCompare(Writer* writer, Reader* reader) : CompositExecutor{ writer, reader } {}

	static const int LOOP_COUNT = 20;
	static const int NUM_LBA_PER_LOOP = 5;

	bool execute(ISsdApp* app, LBA lba, DATA data);
};

class PartialLBAWrite : public CompositExecutor {
public:
	PartialLBAWrite() = default;
	PartialLBAWrite(Writer* writer, Reader* reader) : CompositExecutor{ writer, reader } {}

	static const int LOOP_COUNT = 30;
	static const int NUM_LBA_PER_LOOP = 5;

	bool execute(ISsdApp* app, LBA lba, DATA data);
};

class WriteReadAging : public CompositExecutor {
public:
	WriteReadAging() = default;
	WriteReadAging(Writer* writer, Reader* reader) : CompositExecutor{ writer, reader } {}

	static const int LOOP_COUNT = 200;
	static const int NUM_LBA_PER_LOOP = 2;

	bool execute(ISsdApp* app, LBA lba, DATA data);
};
