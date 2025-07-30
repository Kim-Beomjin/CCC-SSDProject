#pragma once
#include "global_config.h"
#include "executor.h"
#include <string>

using namespace std;

class TestScript
{
public:
	TestScript() = default;
	TestScript(Writer* writer, Reader* reader) : writer{ writer }, reader{ reader } {}

	virtual bool execute(string input, ISsdApp* app) = 0;
	virtual bool IsValidTestScriptName(std::string& input) = 0;

protected:
	Writer* writer;
	Reader* reader;
};

class FullWriteAndReadCompare : public TestScript {
public:
	FullWriteAndReadCompare() = default;
	FullWriteAndReadCompare(Writer* writer, Reader* reader) : TestScript{ writer, reader } {}

	static const int LOOP_COUNT = 20;
	static const int NUM_LBA_PER_LOOP = 5;

	const string SHORT_NAME = "1_";
	const string FULL_NAME = "1_FullWriteAndReadCompare";

	bool execute(string input, ISsdApp* app) override;
	bool IsValidTestScriptName(std::string& input) override;
};

class PartialLBAWrite : public TestScript {
public:
	PartialLBAWrite() = default;
	PartialLBAWrite(Writer* writer, Reader* reader) : TestScript{ writer, reader } {}

	static const int LOOP_COUNT = 30;
	static const int NUM_LBA_PER_LOOP = 5;

	const string SHORT_NAME = "2_";
	const string FULL_NAME = "2_PartialLBAWrite";

	bool execute(string input, ISsdApp* app) override;
	bool IsValidTestScriptName(std::string& input) override;
};

class WriteReadAging : public TestScript {
public:
	WriteReadAging() = default;
	WriteReadAging(Writer* writer, Reader* reader) : TestScript{ writer, reader } {}

	static const int LOOP_COUNT = 200;
	static const int NUM_LBA_PER_LOOP = 2;

	const string SHORT_NAME = "3_";
	const string FULL_NAME = "3_WriteReadAging";

	bool execute(string input, ISsdApp* app) override;
	bool IsValidTestScriptName(std::string& input) override;
};
