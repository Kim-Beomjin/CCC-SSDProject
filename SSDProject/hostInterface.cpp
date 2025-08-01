#include "hostInterface.h"
#include <stdexcept>
#include <iostream>
#include <fstream>

void HostInterface::Execute(int argc, char* argv[])
{
	IProcessor* processor = ProcessorFactory::GetInstance()->CreateProcessor(argc, argv, ssd, bufferManager);
	if (processor != nullptr && processor->LoadParameterAndCheckInvalid(argv[ARGV::LBA_IDX], argv[ARGV::DATA_IDX]))
	{
		processor->Process();
	}
}

//------------------------------ Processor Factory ----------------------------------//

IProcessor* ProcessorFactory::CreateProcessor(int argc, char* argv[], SSD* ssd, BufferManager* bufferManager)
{
	if (_WriteCondition(argc, argv)) return new WriteProcessor(ssd, bufferManager);
	if (_ReadCondition(argc, argv)) return new ReadProcessor(ssd, bufferManager);
	if (_EraseCondition(argc, argv)) return new EraseProcessor(ssd, bufferManager);
	if (_FlushCondition(argc, argv)) return new FlushProcessor(ssd, bufferManager);
	DEBUG_ASSERT(false, "INVALID_PARAMETER");
	std::cout << "SSD GET INVALID PARAMETER!!!!" << "\n";
	return nullptr;
}



bool ProcessorFactory::_WriteCondition(int argc, char* argv[])
{
	return (argc == WRITE_COMMAND_ARG_COUNT && std::string(argv[ARGV::CMD_IDX]) == WRITE_CMD);
}

bool ProcessorFactory::_ReadCondition(int argc, char* argv[])
{
	return (argc == READ_COMMAND_ARG_COUNT && std::string(argv[ARGV::CMD_IDX]) == READ_CMD);
}

bool ProcessorFactory::_EraseCondition(int argc, char* argv[])
{
	return (argc == ERASE_COMMAND_ARG_COUNT && std::string(argv[ARGV::CMD_IDX]) == ERASE_CMD);
}

bool ProcessorFactory::_FlushCondition(int argc, char* argv[])
{
	return (argc == FLUSH_COMMAND_ARG_COUNT && std::string(argv[ARGV::CMD_IDX]) == FLUSH_CMD);
}

//----------------------------------- Processor -------------------------------------//

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


