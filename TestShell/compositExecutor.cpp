#include "CompositExecutor.h"
#ifdef _DEBUG
#include <stdexcept>
#else
#include <random>
#endif

bool CompositExecutor::ReadCompare(ISsdApp* app, LBA lba, DATA expectedData)
{
	DATA ouputData = 0;
	reader->execute("read", lba, ouputData, app);

#ifndef _DEBUG
	if (ouputData != expectedData) return false;
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
			writer->execute("write", lba, inputData, app);
		}

		for (int lba = startLba; lba < endLba; lba++)
		{
			if (!ReadCompare(app, lba, inputData)) return false;
		}
	}

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
			writer->execute("write", lba, inputData, app);
		}

		for (int lba = readStartLba; lba < readEndLba; lba++)
		{
			if (!ReadCompare(app, lba, inputData)) return false;
		}
	}

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
		DATA inputData[0] = rand();
		DATA inputData[1] = rand();
#endif

		for (int index = 0; index < NUM_LBA_PER_LOOP; index++)
		{
			writer->execute("write", ioLba[index], inputData[index], app);
		}

		for (int index = 0; index < NUM_LBA_PER_LOOP; index++)
		{
			if (!ReadCompare(app, ioLba[index], inputData[index])) return false;
		}
	}

	return true;
}
