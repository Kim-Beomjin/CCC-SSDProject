#include "testScript.h"
#ifdef _DEBUG
#include <stdexcept>
#endif

bool TestScript::FullWriteAndReadCompare(string input)
{
	if ((input != "1_") || (input != "1_FullWriteAndReadCompare"))
	{
#ifdef _DEBUG
		throw runtime_error("Invalid Test Script");
#endif
		return false;
	}

	return true;
}
