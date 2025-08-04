#pragma once
#include "global_config.h"
class IProcessor
{
public:
	virtual bool LoadParameterAndCheckInvalid(char*, char*) = 0;
	virtual bool Process() = 0;
};

class ISSD
{
public:
	virtual ~ISSD() {};
	virtual bool Read(LBA lba) = 0;
	virtual bool Write(LBA lba, DATA data) = 0;
	virtual bool Erase(LBA lba, unsigned int size) = 0;
	virtual void Flush(void) = 0;
};

class IBufferStrategy
{
public:
	virtual void ConvertBufferToCmd(std::vector<std::string> &CmdList) = 0;
	virtual void ConvertCmdToBuffer(const std::vector<std::string> CmdList) = 0;
	virtual Buffer GetBuffer(LBA lba) = 0;
};