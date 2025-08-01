#pragma once

class IProcessor
{
public:
	virtual bool LoadParameterAndCheckInvalid(char*, char*) = 0;
	virtual void Process() = 0;
};