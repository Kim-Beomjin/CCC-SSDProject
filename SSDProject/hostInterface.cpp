#include "hostInterface.h"
#include <stdexcept>
#include <iostream>
#include <fstream>

void HostInterface::Execute(int argc, char* argv[])
{
	IProcessor* processor = ProcessorFactory::GetInstance()->CreateProcessor(argc, argv, bufferedSSD);
	if (processor == nullptr || processor->LoadParameterAndCheckInvalid(argv[ARGV::LBA_IDX], argv[ARGV::DATA_IDX]))
	{
		return;
	}
	processor->Process();
}

//------------------------------ Processor Factory ----------------------------------//

IProcessor* ProcessorFactory::CreateProcessor(int argc, char* argv[], BufferedSSD* bufferedSSD)
{
	if (_WriteCondition(argc, argv)) return new WriteProcessor(bufferedSSD);
	if (_ReadCondition(argc, argv)) return new ReadProcessor(bufferedSSD);
	if (_EraseCondition(argc, argv)) return new EraseProcessor(bufferedSSD);
	if (_FlushCondition(argc, argv)) return new FlushProcessor(bufferedSSD);
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
	if (GlobalUtil::IsNegative(lbaStr) || GlobalUtil::IsInvalidLength(dataStr))
	{
		DEBUG_ASSERT(false, "INVALID INPUT PARAMETERS");
		return true;
	}
	try {
		
		lba = GlobalUtil::SafeStoul(lbaStr, DECIMAL_BASE);
		data = GlobalUtil::SafeStoul(dataStr, HEXA_BASE);
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
	bufferedSSD->Write(lba, data);
}

bool ReadProcessor::LoadParameterAndCheckInvalid(char* lbaStr, char* )
{
	if (GlobalUtil::IsNegative(lbaStr))
	{
		DEBUG_ASSERT(false, "INVALID INPUT PARAMETERS");
		return true;
	}
	try {
		lba = GlobalUtil::SafeStoul(lbaStr, DECIMAL_BASE);
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
	bufferedSSD->Read(lba);
}

bool EraseProcessor::LoadParameterAndCheckInvalid(char* lbaStr, char* sizeStr)
{
	if (GlobalUtil::IsNegative(lbaStr) || GlobalUtil::IsNegative(sizeStr))
	{
		DEBUG_ASSERT(false, "INVALID INPUT PARAMETERS");
		return true;
	}
	try {

		lba = GlobalUtil::SafeStoul(lbaStr, DECIMAL_BASE);
		size = GlobalUtil::SafeStoul(sizeStr, DECIMAL_BASE);
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
	bufferedSSD->Erase(lba, size);
}

bool FlushProcessor::LoadParameterAndCheckInvalid(char* , char* )
{
	return false;
}
void FlushProcessor::Process()
{
	bufferedSSD->Flush();
}


