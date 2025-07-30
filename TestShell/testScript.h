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
	Writer* writer;
	Reader* reader;
};