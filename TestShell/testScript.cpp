#include "testScript.h"
#ifdef _DEBUG
#include <stdexcept>
#endif

bool TestScript::FullWriteAndReadCompare(string input, ISsdApp* app)
{
	if (IsValidTestScriptName(input) == false)
	{
#ifdef _DEBUG
		throw runtime_error("Invalid Test Script");
#endif
		return false;
	}

	for (int loop = 0; loop < FIRST_TEST_SCRIPT_LOOP_COUNT; loop++)
	{
		int startLba = loop * FIRST_TEST_SCRIPT_LOOP_LBA;
		int endLba = (loop + 1) * FIRST_TEST_SCRIPT_LOOP_LBA;

		for (int lba = startLba; lba < endLba; lba++)
		{
			writer->execute("write", lba, lba, app);
		}

		for (int lba = startLba; lba < endLba; lba++)
		{
			int data = 0;
			reader->execute("read", lba, data, app);
		}
	}

	return true;
}

bool TestScript::IsValidTestScriptName(std::string& input)
{
	return (input == "1_") || (input == "1_FullWriteAndReadCompare");
}
