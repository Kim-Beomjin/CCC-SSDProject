#include "testScript.h"
#ifdef _DEBUG
#include <stdexcept>
#endif

bool FullWriteAndReadCompare::execute(string input, ISsdApp* app)
{
	if (IsValidTestScriptName(input) == false)
	{
#ifdef _DEBUG
		throw runtime_error("Invalid Test Script");
#endif
		return false;
	}

	for (int loop = 0; loop < LOOP_COUNT; loop++)
	{
		int startLba = loop * NUM_LBA_PER_LOOP;
		int endLba = (loop + 1) * NUM_LBA_PER_LOOP;

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

bool PartialLBAWrite::execute(string input, ISsdApp* app)
{
	if (IsValidTestScriptName(input) == false)
	{
#ifdef _DEBUG
		throw runtime_error("Invalid Test Script");
#endif
		return false;
	}

	int writeLba[5] = { 4, 0, 3, 1, 2 };
	int readStartLba = 0;
	int readEndLba = NUM_LBA_PER_LOOP;

	for (int loop = 0; loop < LOOP_COUNT; loop++)
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

bool WriteReadAging::execute(string input, ISsdApp* app)
{
	if (IsValidTestScriptName(input) == false)
	{
#ifdef _DEBUG
		throw runtime_error("Invalid Test Script");
#endif
		return false;
	}

	int ioLba[2] = { 0, 99 };

	for (int loop = 0; loop < LOOP_COUNT; loop++)
	{
		for (auto lba : ioLba)
		{
			writer->execute("write", lba, lba, app);
		}

		for (auto lba : ioLba)
		{
			int data = 0;
			reader->execute("read", lba, data, app);
		}
	}
}

bool FullWriteAndReadCompare::IsValidTestScriptName(std::string& input)
{
	return (input == SHORT_NAME) || (input == FULL_NAME);
}

bool PartialLBAWrite::IsValidTestScriptName(std::string& input)
{
	return (input == SHORT_NAME) || (input == FULL_NAME);
}

bool WriteReadAging::IsValidTestScriptName(std::string& input)
{
	return (input == SHORT_NAME) || (input == FULL_NAME);
}

