#ifdef _DEBUG
#include <stdexcept>
#endif
#include "nand.h"


bool Nand::Read(const int lba, int &readData)
{
	if (_IsValidParameter(lba) == false)
	{
		return false;
	}
	return true;
}

bool Nand::Write(const int lba, const int writeData)
{
	if (_IsValidParameter(lba) == false)
	{
		return false;
	}
	return true;
}

bool Nand::_IsValidParameter(const int lba)
{
	if (lba < LBA_START_ADDR || lba >= LBA_END_ADDR)
	{
#ifdef _DEBUG
		throw(std::exception("INVALID PARAMETER"));
#else
		return false;
#endif
	}
	return true;
}