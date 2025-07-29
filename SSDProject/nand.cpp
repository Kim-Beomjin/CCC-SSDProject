#ifdef _DEBUG
#include <stdexcept>
#endif
#include "nand.h"

bool Nand::read(int lba, int &out)
{
	if (lba < 0 || lba > 99)
	{
#ifdef _DEBUG
		throw(std::exception("INVALID PARAMETER"));
#else
		return false;
#endif
	}

	return true;
	
}
bool Nand::write(int lba, int data)
{
	if (lba < 0 || lba > 99)
	{
#ifdef _DEBUG
		throw(std::exception("INVALID PARAMETER"));
#else
		return false;
#endif
	}
	return true;
}