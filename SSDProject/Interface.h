#pragma once

class IProcessor
{
public:
	virtual bool LoadParameterAndCheckInvalid(char*, char*) = 0;
	virtual void Process() = 0;
};

class ISSD
{
public:
	virtual void Read(LBA lba) = 0;
	virtual void Write(LBA lba, DATA data) = 0;
	virtual void Erase(LBA lba, unsigned int size) = 0;
	virtual void Flush(void) = 0;
};