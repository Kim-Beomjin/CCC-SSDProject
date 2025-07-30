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
	if (_IsFileExists(NAND_FILE_NAME) == false)
	{
		_CreateFile(NAND_FILE_NAME);
	}
	return true;
}

bool Nand::Write(const int lba, const int writeData)
{
	if (_IsValidParameter(lba) == false)
	{
		return false;
	}
	if (_IsFileExists(NAND_FILE_NAME) == false)
	{
		_CreateFile(NAND_FILE_NAME);
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

bool Nand::_IsFileExists(const std::string& filename)
{
	std::ifstream file(filename);
	return file.good();
}

void Nand::_CreateFile(const std::string& filename)
{
	std::ofstream outFile(filename);

	if (outFile.is_open()) {
		outFile.close();
	}
}
