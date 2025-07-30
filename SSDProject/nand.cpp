#ifdef _DEBUG
#include <stdexcept>
#endif
#include "nand.h"

bool Nand::Read(const LBA lba, DATA& readData)
{
	if (_IsValidParameter(lba) == false)
	{
		return false;
	}

	if (_IsFileValid(NAND_FILE_NAME) == false)
	{
		_CreateFile(NAND_FILE_NAME);
	}
	else
	{
		_ReadFile(NAND_FILE_NAME);
	}
	readData = nandData[lba];
	return true;
}

bool Nand::Write(const LBA lba, const DATA writeData)
{
	if (_IsValidParameter(lba) == false)
	{
		return false;
	}

	if (_IsFileValid(NAND_FILE_NAME) == false)
	{
		_CreateFile(NAND_FILE_NAME);
	}
	else
	{
		_ReadFile(NAND_FILE_NAME);
	}

	nandData[lba] = writeData;
	_WriteFile(NAND_FILE_NAME);
	return true;
}


bool Nand::_IsValidParameter(const LBA lba)
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


bool Nand::_IsFileValid(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	if (file.good() == false)
	{
		return false;
	}
	return (file.tellg() == FILE_SIZE);
}

void Nand::_CreateFile(const std::string& filename)
{
	std::ofstream outFile(filename);
	for (size_t i = LBA_START_ADDR; i < LBA_END_ADDR; ++i) {
		nandData[i] = EMPTY_DATA;
		outFile.write(reinterpret_cast<const char*>(&EMPTY_DATA), sizeof(EMPTY_DATA));
	}
	if (outFile.is_open()) {
		outFile.close();
	}
}

void Nand::_ReadFile(const std::string& filename)
{
	std::ifstream inFile(filename, std::ios::binary);
	if (_IsFileValid(filename) == false) // this case cannot be happen
	{
#ifdef _DEBUG
		throw(std::exception("INVALID FILE"));
#endif
	}

	for (size_t i = LBA_START_ADDR; i < LBA_END_ADDR; ++i) {
		int data;
		inFile.read(reinterpret_cast<char*>(&data), sizeof(data));
		nandData[i] = data;
	}

	inFile.close();
}

void Nand::_WriteFile(const std::string& filename)
{
	std::ofstream outFile(filename);
	for (size_t i = LBA_START_ADDR; i < LBA_END_ADDR; ++i) {
		outFile.write(reinterpret_cast<const char*>(&nandData[i]), sizeof(nandData[i]));
	}
	if (outFile.is_open()) {
		outFile.close();
	}
}