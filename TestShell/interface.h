#pragma once
#include <string>

using std::string;

#define interface struct

const int SSD_MAX_SIZE = 100;

interface ISsdApp {
	ISsdApp() {}

	virtual string Read(int lba) = 0;
	virtual bool Write(int lba, const string& data) = 0;
};

interface IExecutor {
	IExecutor() : app{} {}
	IExecutor(ISsdApp* pApp) : app(pApp) {}
	virtual bool execute(const string& command = "", int lba = 0, const string& data = "") = 0;
	virtual void setApp(ISsdApp* pApp) { app = pApp; }
	ISsdApp* getApp() { return app; }
protected:
	ISsdApp* app;
};

class ExecutorFactory {
public:
	IExecutor* createExecutor(const string command, ISsdApp* pApp = nullptr);
};
