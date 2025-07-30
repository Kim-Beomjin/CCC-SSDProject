#pragma once
#include <string>

using std::string;

#define interface struct

const int SSD_MAX_SIZE = 100;

typedef unsigned int u32;

interface ISsdApp {
	ISsdApp() {}

	virtual u32 Read(u32 lba) = 0;
	virtual bool Write(u32 lba, u32 data) = 0;
};

interface IExecutor {
	IExecutor() : app{} {}
	IExecutor(ISsdApp* pApp) : app(pApp) {}
	virtual bool execute(const string& command = "", u32 lba = 0, u32 data = 0) = 0;
	virtual void setApp(ISsdApp* pApp) { app = pApp; }
	ISsdApp* getApp() { return app; }
protected:
	ISsdApp* app;
};

class ExecutorFactory {
public:
	IExecutor* createExecutor(const string command, ISsdApp* pApp = nullptr);
};
