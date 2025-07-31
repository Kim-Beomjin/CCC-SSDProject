#include "CompositExecutor.h"
#include <iostream>
#ifdef _DEBUG
#include <stdexcept>
#else
#include <random>
#endif

bool CompositExecutor::ReadCompare(ISsdApp* app, LBA lba, DATA expectedData)
{
	bool result = reader->execute(SCRIPT_READ_CMD, lba, expectedData, app);

#ifndef _DEBUG
	if (result == false) cout << "FAIL\n";

	return result;
#endif

	return true;
}


bool FullWriteAndReadCompare::execute(const string& command, LBA lba, DATA data, ISsdApp* app)
{
	for (int loop = 0; loop < LOOP_COUNT; loop++)
	{
		LBA startLba = loop * NUM_LBA_PER_LOOP;
		LBA endLba = (loop + 1) * NUM_LBA_PER_LOOP;
		const DATA inputData = loop;

		for (int lba = startLba; lba < endLba; lba++)
		{
			writer->execute(SCRIPT_WRITE_CMD, lba, inputData, app);
		}

		for (int lba = startLba; lba < endLba; lba++)
		{
			if (!ReadCompare(app, lba, inputData)) return false;
		}
	}

	cout << "PASS\n";
	return true;
}

bool PartialLBAWrite::execute(const string& command, LBA lba, DATA data, ISsdApp* app)
{
	LBA writeLba[5] = { 4, 0, 3, 1, 2 };
	LBA readStartLba = 0;
	LBA readEndLba = NUM_LBA_PER_LOOP;

	for (int loop = 0; loop < LOOP_COUNT; loop++)
	{
		const DATA inputData = loop;

		for (auto lba : writeLba)
		{
			writer->execute(SCRIPT_WRITE_CMD, lba, inputData, app);
		}

		for (int lba = readStartLba; lba < readEndLba; lba++)
		{
			if (!ReadCompare(app, lba, inputData)) return false;
		}
	}

	cout << "PASS\n";
	return true;
}

bool WriteReadAging::execute(const string& command, LBA lba, DATA data, ISsdApp* app)
{
	LBA ioLba[2] = { 0, 99 };

	for (int loop = 0; loop < LOOP_COUNT; loop++)
	{

#ifdef _DEBUG
		DATA inputData[2] = { loop, LOOP_COUNT - loop };
#else
		DATA inputData[2];
		inputData[0] = rand();
		inputData[1] = rand();
#endif

		for (int index = 0; index < NUM_LBA_PER_LOOP; index++)
		{
			writer->execute(SCRIPT_WRITE_CMD, ioLba[index], inputData[index], app);
		}

		for (int index = 0; index < NUM_LBA_PER_LOOP; index++)
		{
			if (!ReadCompare(app, ioLba[index], inputData[index])) return false;
		}
	}

	cout << "PASS\n";
	return true;
}
