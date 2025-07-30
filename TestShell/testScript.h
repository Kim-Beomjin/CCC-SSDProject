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

	bool FullWriteAndReadCompare(string input, ISsdApp* app);
	bool PartialLBAWrite(string input, ISsdApp* app);

	const string FIRST_TEST_SCRIPT_NAME_SHORT = "1_";
	const string FIRST_TEST_SCRIPT_NAME_FULL = "1_FullWriteAndReadCompare";
	static const int FIRST_TEST_SCRIPT_LOOP_COUNT = 20;
	static const int FIRST_TEST_SCRIPT_LOOP_LBA = 5;

	const string SECOND_TEST_SCRIPT_NAME_SHORT = "2_";
	const string SECOND_TEST_SCRIPT_NAME_FULL = "2_PartialLBAWrite";
	static const int SECOND_TEST_SCRIPT_LOOP_COUNT = 30;
	static const int SECOND_TEST_SCRIPT_LOOP_LBA = 5;

private:
	bool IsValidTestScript1Name(std::string& input);
	bool IsValidTestScript2Name(std::string& input);


	Writer* writer;
	Reader* reader;
};