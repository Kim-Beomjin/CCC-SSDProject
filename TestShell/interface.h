#pragma once
#include <memory>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::shared_ptr;

#define interface struct

typedef unsigned int u32;

interface ISsdApp {
	virtual bool Read(u32 lba) = 0;
	virtual bool Write(u32 lba, u32 data) = 0;
	virtual bool Erase(u32 lba, u32 size) = 0;
	virtual bool Flush() = 0;
};

interface IExecutor {
	virtual bool IsValidCommand(const vector<string>& tokens) { return false; }
	virtual bool execute(ISsdApp* pApp = nullptr, u32 lba = 0, u32 data = 0) = 0;
};

interface ICommandParserBridge {
	virtual bool ParseCommand(const string& cmd) = 0;
	virtual bool ExecuteSsdUsingParsedCommand(ISsdApp* app) = 0;
};

class IExecutorFactory {
public:
	virtual shared_ptr<IExecutor> createExecutor(const string& command) = 0;
};
