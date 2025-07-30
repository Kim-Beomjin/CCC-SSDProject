#include "testScript.h"
#ifdef _DEBUG
#include <stdexcept>
#endif

bool TestScript::FullWriteAndReadCompare(string input, ISsdApp* app)
{
	if (IsValidTestScript1Name(input) == false)
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

bool TestScript::PartialLBAWrite(string input, ISsdApp* app)
{
	if (IsValidTestScript2Name(input) == false)
	{
#ifdef _DEBUG
		throw runtime_error("Invalid Test Script");
#endif
		return false;
	}

	int writeLba[5] = { 4, 0, 3, 1, 2 };
	int readStartLba = 0;
	int readEndLba = SECOND_TEST_SCRIPT_LOOP_LBA;

	for (int loop = 0; loop < SECOND_TEST_SCRIPT_LOOP_COUNT; loop++)
	{

		for (auto lba : writeLba)
		{
			writer->execute("write", lba, lba, app);
		}

		for (int lba = readStartLba; lba < readEndLba; lba++)
		{
			int data = 0;
			reader->execute("read", lba, data, app);
		}
	}
}

bool TestScript::IsValidTestScript1Name(std::string& input)
{
	return (input == FIRST_TEST_SCRIPT_NAME_SHORT) || (input == FIRST_TEST_SCRIPT_NAME_FULL);
}

bool TestScript::IsValidTestScript2Name(std::string& input)
{
	return (input == SECOND_TEST_SCRIPT_NAME_SHORT) || (input == SECOND_TEST_SCRIPT_NAME_FULL);
}

