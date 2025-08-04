#pragma once
#include "global_config.h"
class IProcessor
{
public:
	virtual bool LoadParameterAndCheckValid(char*, char*) = 0;
	virtual bool Process() = 0;
};

class ISSD
{
public:
	virtual bool Read(LBA lba) = 0;
	virtual bool Write(LBA lba, DATA data) = 0;
	virtual bool Erase(LBA lba, unsigned int size) = 0;
	virtual void Flush(void) = 0;
};