#pragma once
#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

#define interface struct

const int SSD_MAX_SIZE = 100;

interface ISsdApp {
	ISsdApp() : ssd_nand{} {}

	virtual string Read(int lba) = 0;
	virtual bool Write(int lba, const string& data) = 0;
protected:
	unordered_map<int, string> ssd_nand;
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
