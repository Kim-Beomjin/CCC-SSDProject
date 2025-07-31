#include "CompositExecutor.h"
#include "utils.h"
#include <iostream>
#ifdef _DEBUG
#include <stdexcept>
#else
#include <random>
#endif

bool CompositExecutor::ReadCompare(ISsdApp* app, LBA lba, DATA expectedData)
{
	if (comparer->execute(app, lba, expectedData)) return true;

#ifndef _DEBUG
	cout << "FAIL\n";
	return false;
#endif

	return true;
}


bool FullWriteAndReadCompare::execute(ISsdApp* app, LBA lba, DATA data)
{
	for (int loop = 0; loop < LOOP_COUNT; loop++)
	{
		LBA startLba = loop * NUM_LBA_PER_LOOP;
		LBA endLba = (loop + 1) * NUM_LBA_PER_LOOP;
		const DATA inputData = loop;

		for (int lba = startLba; lba < endLba; lba++)
		{
			writer->execute(app, lba, inputData);
		}

		for (int lba = startLba; lba < endLba; lba++)
		{
			if (!ReadCompare(app, lba, inputData)) return false;
		}
	}

	cout << "PASS\n";
	return true;
}

bool PartialLBAWrite::execute(ISsdApp* app, LBA lba, DATA data)
{
	LBA writeLba[5] = { 4, 0, 3, 1, 2 };
	LBA readStartLba = 0;
	LBA readEndLba = NUM_LBA_PER_LOOP;

	for (int loop = 0; loop < LOOP_COUNT; loop++)
	{
		const DATA inputData = loop;

		for (auto lba : writeLba)
		{
			writer->execute(app, lba, inputData);
		}

		for (int lba = readStartLba; lba < readEndLba; lba++)
		{
			if (!ReadCompare(app, lba, inputData)) return false;
		}
	}

	cout << "PASS\n";
	return true;
}

bool WriteReadAging::execute(ISsdApp* app, LBA lba, DATA data)
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
			writer->execute(app, ioLba[index], inputData[index]);
		}

		for (int index = 0; index < NUM_LBA_PER_LOOP; index++)
		{
			if (!ReadCompare(app, ioLba[index], inputData[index])) return false;
		}
	}

	cout << "PASS\n";
	return true;
}

bool EraseAndWriteAging::execute(ISsdApp* app, LBA lba, DATA data)
{
	return false;
}

