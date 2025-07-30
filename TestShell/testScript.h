#pragma once
#include "executor.cpp"
#include <string>

using namespace std;

class TestScript
{
public:
	TestScript() = default;
	TestScript(Writer* writer, Reader* reader) : writer{ writer }, reader{ reader } {}

	bool FullWriteAndReadCompare(string input);

private:
	Writer* writer;
	Reader* reader;
};