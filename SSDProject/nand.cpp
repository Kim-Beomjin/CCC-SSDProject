#ifdef _DEBUG
#include <stdexcept>
#endif
#include "nand.h"
#include <fstream>
#include <string>

bool Nand::Read(const LBA lba, DATA& readData)
{
	if (IsInvalidParameter(lba))
	{
		return false;
	}
	_LoadNandFromFile(NAND_FILE_NAME);
	readData = nandData[lba];
	return true;
}

bool Nand::Write(const LBA lba, const DATA writeData)
{
	if (IsInvalidParameter(lba))
	{
		return false;
	}
	_LoadNandFromFile(NAND_FILE_NAME);
	nandData[lba] = writeData;
	_DumpNandToFile(NAND_FILE_NAME);
	return true;
}

bool Nand::Erase(const LBA lba, const ERASE_SIZE erase_size)
{
	if (IsInvalidParameter(lba, erase_size))
	{
		return false;
	}
	_LoadNandFromFile(NAND_FILE_NAME);
	memset(nandData + lba, EMPTY_DATA, sizeof(DATA) * erase_size);
	_DumpNandToFile(NAND_FILE_NAME);
	return true;
}

bool Nand::IsInvalidParameter(const LBA lba, const ERASE_SIZE erase_size)
{
	if (lba >= LBA_END_ADDR || lba + erase_size > LBA_END_ADDR || erase_size > MAX_ERASE_SIZE)
	{
		DEBUG_ASSERT(false, "INVALID_PARAMETER");
		return true;
	}
	return false;
}

void Nand::_LoadNandFromFile(const std::string& filename)
{
	if (_IsInvalidFile(NAND_FILE_NAME))
	{
		_CreateFile(NAND_FILE_NAME);
	}
	_ReadFile(NAND_FILE_NAME);
}

void Nand::_DumpNandToFile(const std::string& filename)
{
	std::ofstream outFile(filename, std::ios::binary);
	for (LBA lba = LBA_START_ADDR; lba < LBA_END_ADDR; ++lba) {
		outFile.write(reinterpret_cast<const char*>(&nandData[lba]), sizeof(nandData[lba]));
	}
}


bool Nand::_IsInvalidFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	if (!file.good())
	{
		return true;
	}
	return ((file.tellg() == FILE_SIZE) == false);
}

void Nand::_CreateFile(const std::string& filename)
{
	for(LBA lba = LBA_START_ADDR; lba < LBA_END_ADDR; ++lba)
	{
		nandData[lba] = EMPTY_DATA; // explicit initialization
	}
	_DumpNandToFile(filename);
}

void Nand::_ReadFile(const std::string& filename)
{
	if (_IsInvalidFile(filename)) // this case cannot be happen
	{
		DEBUG_ASSERT(false, "INVALID FILE");
	}

	std::ifstream inFile(filename, std::ios::binary);
	for (LBA lba = LBA_START_ADDR; lba < LBA_END_ADDR; ++lba) {
		int data;
		inFile.read(reinterpret_cast<char*>(&data), sizeof(data));
		nandData[lba] = data;
	}
}
