#include "hostInterface.h"
#include <stdexcept>

void HostInterface::Execute(int argc, char* argv[])
{

	if (_WriteCondition(argc,argv))
	{
		if (_LoadWriteParameterAndCheckInvalid(argv[ARGV::LBA_IDX], argv[ARGV::DATA_IDX])) {
			return;
		}
		ssd->write(lba, data);
	}
	else if (_ReadCondition(argc, argv))
	{
		if (_LoadReadParameterAndCheckInvalid(argv[ARGV::LBA_IDX])) {
			return;
		}
		ssd->read(lba);
	}
	else {
		DEBUG_ASSERT(false, "INVALID INPUT PARAMETERS");
	}
}

bool HostInterface::_WriteCondition(int argc, char* argv[])
{
	return (argc == WRITE_COMMAND_ARG_COUNT && argv[ARGV::CMD_IDX] == WRITE_CMD);
}

bool HostInterface::_ReadCondition(int argc, char* argv[])
{
	return (argc == READ_COMMAND_ARG_COUNT && argv[ARGV::CMD_IDX] == READ_CMD);
}


bool HostInterface::_LoadWriteParameterAndCheckInvalid(char* lbaStr, char* dataStr)
{
	if (_IsNegative(lbaStr) || _IsInvalidLength(dataStr))
	{
		DEBUG_ASSERT(false, "INVALID INPUT PARAMETERS");
		return false;
	}
	try {
		lba = _SafeStoul(lbaStr);
		data = _SafeStoul(dataStr);
	}
	catch (std::exception)
	{
		DEBUG_ASSERT(false, "INVALID INPUT PARAMETERS");
		return false;
	}
	return true;
}

bool HostInterface::_LoadReadParameterAndCheckInvalid(char* lbaStr)
{
	if (_IsNegative(lbaStr))
	{
		DEBUG_ASSERT(false, "INVALID INPUT PARAMETERS");
		return false;
	}
	try {
		lba = _SafeStoul(lbaStr);
	}
	catch (std::exception)
	{
		DEBUG_ASSERT(false, "INVALID INPUT PARAMETERS");
		return false;
	}
	return true;
}

bool HostInterface::_IsNegative(char* lbaStr)
{
	return (lbaStr[0] == '-');
}

bool HostInterface::_IsInvalidLength(char* dataStr)
{
	return (strlen(dataStr) != 10 || dataStr[0] != ZERO || !(dataStr[1] == LARGE_EX || dataStr[1] == SMALL_EX));
}

unsigned int HostInterface::_SafeStoul(char* str)
{
	size_t idx;
	unsigned int ret = std::stoul(str, &idx, 0);
	if (idx != strlen(str)) {
		DEBUG_ASSERT(false, "INVALID INPUT PARAMETERS");
	}
	return ret;
}

