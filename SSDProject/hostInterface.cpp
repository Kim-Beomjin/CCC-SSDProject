#include "hostInterface.h"
#include <stdexcept>
#include <iostream>
#include <fstream>

void HostInterface::Execute(int argc, char* argv[])
{
	if (_WriteCondition(argc,argv))
	{
		IProcessor* processor = new WriteProcessor(ssd, bufferManager);
		if (processor->LoadParameterAndCheckInvalid(argv[ARGV::LBA_IDX], argv[ARGV::DATA_IDX]))
		{
			processor->Process();
		}
	}
	else if (_ReadCondition(argc, argv))
	{
		IProcessor* processor = new ReadProcessor(ssd, bufferManager);
		if (processor->LoadParameterAndCheckInvalid(argv[ARGV::LBA_IDX], nullptr))
		{
			processor->Process();
		}
	}
	else if (_EraseCondition(argc, argv))
	{
		IProcessor* processor = new EraseProcessor(ssd, bufferManager);
		if (processor->LoadParameterAndCheckInvalid(argv[ARGV::LBA_IDX], argv[ARGV::SIZE_IDX]))
		{
			processor->Process();
		}
	}
	else if (_FlushCondition(argc, argv))
	{
		IProcessor* processor = new FlushProcessor(ssd, bufferManager);
		if (processor->LoadParameterAndCheckInvalid(nullptr, nullptr))
		{
			processor->Process();
		}
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



bool WriteProcessor::LoadParameterAndCheckInvalid(char* lbaStr, char* dataStr)
{
	if (hostUtil.IsNegative(lbaStr) || hostUtil.IsInvalidLength(dataStr))
	{
		DEBUG_ASSERT(false, "INVALID INPUT PARAMETERS");
		return true;
	}
	try {
		
		lba = hostUtil.SafeStoul(lbaStr, 10);
		data = hostUtil.SafeStoul(dataStr, 16);
	}
	catch (std::exception)
	{
		DEBUG_ASSERT(false, "INVALID INPUT PARAMETERS");
		return true;
	}
	return false;
}

void WriteProcessor::Process()
{
	if (lba < LBA_END_ADDR)
	{
		bufferManager->BufferWrite(lba, data);
		return;
	}
	ssd->Write(lba, data);
}


bool ReadProcessor::LoadParameterAndCheckInvalid(char* lbaStr, char* )
{
	if (hostUtil.IsNegative(lbaStr))
	{
		DEBUG_ASSERT(false, "INVALID INPUT PARAMETERS");
		return true;
	}
	try {
		lba = hostUtil.SafeStoul(lbaStr, 10);
	}
	catch (std::exception)
	{
		DEBUG_ASSERT(false, "INVALID INPUT PARAMETERS");
		return true;
	}
	return false;
}

void ReadProcessor::Process()
{
	DATA readData;
	if (lba < LBA_END_ADDR && bufferManager->BufferRead(lba, readData))
	{
		ssd->UpdateOutputFileUsingData(readData);
		return;
	}
	ssd->Read(lba);
}


bool EraseProcessor::LoadParameterAndCheckInvalid(char* lbaStr, char* sizeStr)
{
	if (hostUtil.IsNegative(lbaStr) || hostUtil.IsNegative(sizeStr))
	{
		DEBUG_ASSERT(false, "INVALID INPUT PARAMETERS");
		return true;
	}
	try {

		lba = hostUtil.SafeStoul(lbaStr, 10);
		size = hostUtil.SafeStoul(sizeStr, 10);
	}
	catch (std::exception)
	{
		DEBUG_ASSERT(false, "INVALID INPUT PARAMETERS");
		return true;
	}
	return false;
}

void EraseProcessor::Process()
{
	if (lba + size <= LBA_END_ADDR && size <= 10)
	{
		bufferManager->BufferErase(lba, size);
		return;
	}
	ssd->Erase(lba, size);
}


bool FlushProcessor::LoadParameterAndCheckInvalid(char* , char* )
{
	return false;
}
void FlushProcessor::Process()
{
	bufferManager->BufferFlush();
}


