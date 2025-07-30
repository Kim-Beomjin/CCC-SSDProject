#include "CompositExecutor.h"
#ifdef _DEBUG
#include <stdexcept>
#endif

bool FullWriteAndReadCompare::execute(const string& command, LBA lba, DATA data, ISsdApp* app)
{
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

bool PartialLBAWrite::execute(const string& command, LBA lba, DATA data, ISsdApp* app)
{
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

	return true;
}

bool WriteReadAging::execute(const string& command, LBA lba, DATA data, ISsdApp* app)
{
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

	return true;
}
