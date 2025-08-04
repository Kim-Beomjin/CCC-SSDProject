#include "hostInterface.h"
#include <stdexcept>
#include <iostream>
#include <fstream>

void HostInterface::Execute(int argc, char* argv[])
{
	IProcessor* processor = ProcessorFactory::GetInstance()->CreateProcessor(argc, argv, ssd);
	if (processor == nullptr || (processor->LoadParameterAndCheckInvalid(argv[ARGV::LBA_IDX], argv[ARGV::DATA_IDX]) == false))
	{
		return;
	}
	if (processor->Process() == false)
	{
		std::cout << "SSD Process Fail\n";
	}
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
		return false;
	}
	try {
		
		lba = GlobalUtil::TryLoadDecimalInput(lbaStr);
		data = GlobalUtil::TryLoadHexaInput(dataStr);
	}
	catch (std::exception)
	{
		DEBUG_ASSERT(false, "INVALID INPUT PARAMETERS");
		return false;
	}
	return true;
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
		return false;
	}
	try {
		lba = GlobalUtil::TryLoadDecimalInput(lbaStr);
	}
	catch (std::exception)
	{
		DEBUG_ASSERT(false, "INVALID INPUT PARAMETERS");
		return false;
	}
	return true;
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
		return false;
	}
	try {

		lba = GlobalUtil::TryLoadDecimalInput(lbaStr);
		size = GlobalUtil::TryLoadDecimalInput(sizeStr);
	}
	catch (std::exception)
	{
		DEBUG_ASSERT(false, "INVALID INPUT PARAMETERS");
		return false;
	}
	return true;
}
bool EraseProcessor::Process()
{
	return ssd->Erase(lba, size);
}

bool FlushProcessor::LoadParameterAndCheckInvalid(char* , char* )
{
	return true;
}
bool FlushProcessor::Process()
{
	ssd->Flush();
	return true;
}


