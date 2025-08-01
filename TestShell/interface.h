#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

using std::string;
using std::vector;
using std::unordered_map;

#define interface struct

typedef unsigned int u32;

interface ISsdApp {
	virtual u32 Read(u32 lba) = 0;
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
	virtual IExecutor* createExecutor(const string& command) = 0;
protected:
	using Creator = std::function<IExecutor* ()>;
	unordered_map<string, std::unique_ptr<IExecutor>> executorCache;

	virtual IExecutor* getExecutorFromCache(const string& command);
	virtual IExecutor* getExecutorFromMap(const string& command);
};

class CompositExecutorFactory : public IExecutorFactory {
public:
	IExecutor* createExecutor(const string& command) override;
protected:
	IExecutor* getExecutorFromCache(const string& command) override;
	IExecutor* getExecutorFromMap(const string& command) override;
private:
	static const unordered_map<string, Creator> factoryMap;
};

class ExecutorFactory : public IExecutorFactory {
public:
	IExecutor* createExecutor(const string& command);
protected:
	IExecutor* getExecutorFromCache(const string& command) override;
	IExecutor* getExecutorFromMap(const string& command) override;
private:
	static const unordered_map<string, Creator> factoryMap;
};


