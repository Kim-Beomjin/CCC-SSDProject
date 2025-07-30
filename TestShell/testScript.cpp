#include "testScript.h"
#ifdef _DEBUG
#include <stdexcept>
#endif

bool TestScript::FullWriteAndReadCompare(string input, ISsdApp* app)
{
	if ((input != "1_") && (input != "1_FullWriteAndReadCompare"))
	{
#ifdef _DEBUG
		throw runtime_error("Invalid Test Script");
#endif
		return false;
	}

	for (int step = 0; step < 20; step++)
	{
		int startLba = step * 5;
		int endLba = (step + 1) * 5;

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
