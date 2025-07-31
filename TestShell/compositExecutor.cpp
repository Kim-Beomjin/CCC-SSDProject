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
		const DATA writeData = loop;

		for (LBA writeLba = startLba; writeLba < endLba; writeLba++)
		{
			writer->execute(app, writeLba, writeData);
		}

		for (LBA readLba = startLba; readLba < endLba; readLba++)
		{
			if (!ReadCompare(app, readLba, writeData)) return false;
		}
	}

	cout << "PASS\n";
	return true;
}

bool PartialLBAWrite::execute(ISsdApp* app, LBA lba, DATA data)
{
	LBA writeLbaRange[5] = { 4, 0, 3, 1, 2 };
	LBA readStartLba = 0;
	LBA readEndLba = NUM_LBA_PER_LOOP;

	for (int loop = 0; loop < LOOP_COUNT; loop++)
	{
		const DATA writeData = loop;

		for (auto writeLba : writeLbaRange)
		{
			writer->execute(app, writeLba, writeData);
		}

		for (DATA readLba = readStartLba; readLba < readEndLba; readLba++)
		{
			if (!ReadCompare(app, readLba, writeData)) return false;
		}
	}

	cout << "PASS\n";
	return true;
}

bool WriteReadAging::execute(ISsdApp* app, LBA lba, DATA data)
{
	LBA lbaRange[2] = { 0, 99 };

	for (int loop = 0; loop < LOOP_COUNT; loop++)
	{

#ifdef _DEBUG
		DATA writeData[2] = { loop, LOOP_COUNT - loop };
#else
		DATA writeData[2];
		writeData[0] = rand();
		writeData[1] = rand();
#endif

		for (int index = 0; index < NUM_LBA_PER_LOOP; index++)
		{
			writer->execute(app, lbaRange[index], writeData[index]);
		}

		for (int index = 0; index < NUM_LBA_PER_LOOP; index++)
		{
			if (!ReadCompare(app, lbaRange[index], writeData[index])) return false;
		}
	}

	cout << "PASS\n";
	return true;
}

bool EraseAndWriteAging::execute(ISsdApp* app, LBA lba, DATA data)
{
	const DATA erasedData = 0;
	const LBA loopStartLBA = 2;
	const LBA ssdMaxLBA = 99;
	
	eraser->execute(app, 0, 3);
	for (int loop = 0; loop < LOOP_COUNT; loop++)
	{
		const DATA writeData = loop;
		const DATA overWriteData = LOOP_COUNT - loop;

		for (LBA testLba = loopStartLBA; testLba < ssdMaxLBA; testLba += (NUM_LBA_PER_LOOP - 1))
		{
			LBA startLba = testLba;
			LBA endLba = testLba + NUM_LBA_PER_LOOP;

			writer->execute(app, startLba, writeData);
			writer->execute(app, startLba, overWriteData);
			eraser->execute(app, startLba, NUM_LBA_PER_LOOP);

			for (LBA readLba = startLba; readLba < endLba; readLba++)
			{
				if (!ReadCompare(app, readLba, erasedData)) return false;
			}
		}
	}

	cout << "PASS\n";
	return true;
}

