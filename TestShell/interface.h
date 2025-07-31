#pragma once
#include <string>

using std::string;

#define interface struct

const int SSD_MAX_SIZE = 100;

typedef unsigned int u32;

interface ISsdApp {
	virtual u32 Read(u32 lba) = 0;
	virtual bool Write(u32 lba, u32 data) = 0;
};

interface IExecutor {
	virtual bool execute(ISsdApp* pApp = nullptr, u32 lba = 0, u32 data = 0) = 0;
};

class ICommandParserBridge
{
public:
	virtual bool ParseCommand(const string& cmd) = 0;
	virtual bool ExecuteSsdUsingParsedCommand(ISsdApp* app) = 0;
};

class ExecutorFactory {
public:
	IExecutor* createExecutor(const string command);
};
