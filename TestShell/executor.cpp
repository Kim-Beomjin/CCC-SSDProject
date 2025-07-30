#include <iostream>
#include <exception>
#include "interface.h"

using std::string;
using std::cout;
using std::endl;

class SsdApp : public ISsdApp {
public:
	u32 Read(u32 lba) override {
		// TODO: need to execute existing exe file of ssd
	}

	bool Write(u32 lba, u32 data) override {
		// TODO: need to execute existing exe file of ssd
	}
};

class Writer : public IExecutor {
public:
	Writer(ISsdApp* pApp) : IExecutor(pApp) {}
	bool execute(const string& command, u32 lba, u32 data) override {
		if (command == "write") {
			app->Write(lba, data);
			return true;
		}
		else if (command == "fullwrite") {
			for (int i = 0; i < SSD_MAX_SIZE; ++i) app->Write(i, data);
			return true;
		}
		return false;
	}
};

class Reader : public IExecutor {
public:
	Reader(ISsdApp* pApp) : IExecutor(pApp) {}
	bool execute(const string& command, u32 lba, u32 data) override {
		if (command == "read") {
			app->Read(lba);
			return true;
		}
		else if (command == "fullread") {
			for (int i = 0; i < SSD_MAX_SIZE; ++i) app->Read((u32)i);
			return true;
		}
		return false;
	}
};

class Helper : public IExecutor {
public:
	bool execute(const string& command, u32 lba, u32 data) override {
		cout << "팀명: CCC(Clean Code Collective) \n";
		cout << "팀장 : 김범진 / 팀원 : 김경민, 김윤진, 김율곤, 정지윤\n\n";
		cout << "Command 설명:\n";
		cout << "write: 필요 파라미터 lba(0~99 중), data(32bit 16진수 값) - ssd의 lba cell에 data를 저장한다.\n";
		cout << "fullwrite: 필요 파라미터 data(32bit 16진수 값) - ssd의 모든 cell에 data를 저장한다.\n";
		cout << "read: 필요 파라미터 lba(0~99 중) - ssd의 lba cell의 값을 읽어온다.\n";
		cout << "fullread: 필요 파라미터 NONE - ssd의 모든 cell의 값을 읽어온다\n";
		cout << "help: 필요 파라미터 NONE - ssd에 필요한 command 등 모든 정보에 대한 도움말을 요청한다\n";
		cout << "exit: 필요 파라미터 NONE - 하려던 거 끝내고 바로 종료한다\n" << endl;

		return true;
	}
};

class Exiter : public IExecutor {
public:
	bool execute(const string& command, u32 lba, u32 data) override {
		cout << "Exit!!" << endl;
		return true;
	}
};

IExecutor* ExecutorFactory::createExecutor(const string command, ISsdApp* pApp) {
	if (command == "write" || command == "fullwrite") {
		return new Writer(pApp);
	}
	else if (command == "read" || command == "fullread") {
		return new Reader(pApp);
	}
	else if (command == "help") {
		return new Helper();
	}
	else if (command == "exit") {
		return new Exiter();
	}

	cout << "Invalid command:" << command << endl;
	return nullptr;
}

