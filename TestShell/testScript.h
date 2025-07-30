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

private:
	bool IsValidTestScriptName(std::string& input);

	const int FIRST_TEST_SCRIPT_LOOP_COUNT = 20;
	const int FIRST_TEST_SCRIPT_LOOP_LBA = 5;

	Writer* writer;
	Reader* reader;
};