#include "hostInterface.h"
#include <stdexcept>
#include <iostream>
#include <fstream>

void HostInterface::Execute(int argc, char* argv[])
{
#ifdef _DEBUG
	ssd = new BufferedSSD();
#endif
	IProcessor* processor = ProcessorFactory::GetInstance()->CreateProcessor(argc, argv, ssd);
	if (processor == nullptr || processor->LoadParameterAndCheckInvalid(argv[ARGV::LBA_IDX], argv[ARGV::DATA_IDX]))
	{
		return;
	}
	if (processor->Process() == false)
	{
		std::cout << "SSD Process Fail\n";
	}
	ssd->~ISSD();
}

//------------------------------ Processor Factory ----------------------------------//

IProcessor* ProcessorFactory::CreateProcessor(int argc, char* argv[], ISSD* ssd)
{
	if (_WriteCondition(argc, argv)) return new WriteProcessor(ssd);
	if (_ReadCondition(argc, argv)) return new ReadProcessor(ssd);
	if (_EraseCondition(argc, argv)) return new EraseProcessor(ssd);
	if (_FlushCondition(argc, argv)) return new FlushProcessor(ssd);
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
bool WriteProcessor::Process()
{
	return ssd->Write(lba, data);
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
bool ReadProcessor::Process()
{
	return ssd->Read(lba);
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
bool EraseProcessor::Process()
{
	return ssd->Erase(lba, size);
}

bool FlushProcessor::LoadParameterAndCheckInvalid(char* , char* )
{
	return false;
}
bool FlushProcessor::Process()
{
	ssd->Flush();
	return true;
}


