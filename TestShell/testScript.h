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


private:
	bool IsValidTestScript1Name(std::string& input);
	bool IsValidTestScript2Name(std::string& input);

	const int FIRST_TEST_SCRIPT_LOOP_COUNT = 20;
	const int FIRST_TEST_SCRIPT_LOOP_LBA = 5;

	const int SECOND_TEST_SCRIPT_LOOP_COUNT = 30;
	const int SECOND_TEST_SCRIPT_LOOP_LBA = 5;

	Writer* writer;
	Reader* reader;
};