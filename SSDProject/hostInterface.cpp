#include "hostInterface.h"
#include <stdexcept>
#include <iostream>
#include <fstream>

void HostInterface::Execute(int argc, char* argv[])
{
	if (_WriteCondition(argc,argv))
	{
		if (_LoadWriteParameterAndCheckInvalid(argv[ARGV::LBA_IDX], argv[ARGV::DATA_IDX])) {
			return;
		}
		if (lba < LBA_END_ADDR)
		{
			bufferManager->WriteBuffer(argv[ARGV::CMD_IDX], argv[ARGV::LBA_IDX], argv[ARGV::DATA_IDX]);
			return;
		}
		ssd->Write(lba, data);
	}
	else if (_ReadCondition(argc, argv))
	{
		if (_LoadReadParameterAndCheckInvalid(argv[ARGV::LBA_IDX], argv[ARGV::DATA_IDX])) {
			return;
		}
		if (lba < LBA_END_ADDR && bufferManager->CheckAndReadBuffer(lba, data))
		{
			ssd->UpdateOutputFileUsingData(data);
			return;
		}
		ssd->Read(lba);
	}
	else if (_EraseCondition(argc, argv))
	{
		if (_LoadEraseParameterAndCheckInvalid(argv[ARGV::LBA_IDX], argv[ARGV::LENGTH_IDX])) {
			return;
		}
		if (lba + length <= LBA_END_ADDR && length <= 10)
		{
			bufferManager->WriteBuffer(argv[ARGV::CMD_IDX], argv[ARGV::LBA_IDX], argv[ARGV::LENGTH_IDX]);
			return;
		}
		ssd->Erase(lba, length);
	}
	else if (_FlushCondition(argc, argv))
	{
		if (_LoadFlushParameterAndCheckInvalid(argv[ARGV::LBA_IDX], argv[ARGV::DATA_IDX])) {
			return;
		}
		bufferManager->Flush();
	}
	else {
		DEBUG_ASSERT(false, "INVALID INPUT PARAMETERS");
	}
}

bool HostInterface::_WriteCondition(int argc, char* argv[])
{
	return (argc == WRITE_COMMAND_ARG_COUNT && std::string(argv[ARGV::CMD_IDX]) == WRITE_CMD);
}

bool HostInterface::_ReadCondition(int argc, char* argv[])
{
	return (argc == READ_COMMAND_ARG_COUNT && std::string(argv[ARGV::CMD_IDX]) == READ_CMD);
}

bool HostInterface::_EraseCondition(int argc, char* argv[])
{
	return (argc == ERASE_COMMAND_ARG_COUNT && std::string(argv[ARGV::CMD_IDX]) == ERASE_CMD);
}

bool HostInterface::_FlushCondition(int argc, char* argv[])
{
	return (argc == FLUSH_COMMAND_ARG_COUNT && std::string(argv[ARGV::CMD_IDX]) == FLUSH_CMD);
}


bool HostInterface::_LoadWriteParameterAndCheckInvalid(char* lbaStr, char* dataStr)
{
	if (_IsNegative(lbaStr) || _IsInvalidLength(dataStr))
	{
		DEBUG_ASSERT(false, "INVALID INPUT PARAMETERS");
		return true;
	}
	try {
		
		lba = _SafeStoul(lbaStr, 10);
		data = _SafeStoul(dataStr, 16);
	}
	catch (std::exception)
	{
		DEBUG_ASSERT(false, "INVALID INPUT PARAMETERS");
		return true;
	}
	return false;
}

bool HostInterface::_LoadReadParameterAndCheckInvalid(char* lbaStr, char* )
{
	if (_IsNegative(lbaStr))
	{
		DEBUG_ASSERT(false, "INVALID INPUT PARAMETERS");
		return true;
	}
	try {
		lba = _SafeStoul(lbaStr, 10);
	}
	catch (std::exception)
	{
		DEBUG_ASSERT(false, "INVALID INPUT PARAMETERS");
		return true;
	}
	return false;
}

bool HostInterface::_LoadEraseParameterAndCheckInvalid(char* lbaStr, char* lengthStr)
{
	if (_IsNegative(lbaStr) || _IsNegative(lengthStr))
	{
		DEBUG_ASSERT(false, "INVALID INPUT PARAMETERS");
		return true;
	}
	try {

		lba = _SafeStoul(lbaStr, 10);
		length = _SafeStoul(lengthStr, 10);
	}
	catch (std::exception)
	{
		DEBUG_ASSERT(false, "INVALID INPUT PARAMETERS");
		return true;
	}
	return false;
}

bool HostInterface::_LoadFlushParameterAndCheckInvalid(char* , char* )
{
	return false;
}

bool HostInterface::_IsNegative(char* lbaStr)
{
	return (std::string(lbaStr)[0] == '-');
}

bool HostInterface::_IsInvalidLength(char* dataStr)
{
	
	return (std::string(dataStr).size() != 10 || std::string(dataStr)[0] != ZERO || !(std::string(dataStr)[1] == LARGE_EX || std::string(dataStr)[1] == SMALL_EX));
}

unsigned int HostInterface::_SafeStoul(char* str, int base)
{
	size_t idx;
	unsigned int ret = std::stoul(std::string(str), &idx, base);
	if (idx != std::string(str).size()) {
		throw(std::exception("INVALID INPUT PARAMETERS"));
	}
	return ret;
}

