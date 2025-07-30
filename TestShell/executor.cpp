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
		cout << "����: CCC(Clean Code Collective) \n";
		cout << "���� : ����� / ���� : ����, ������, ������, ������\n\n";
		cout << "Command ����:\n";
		cout << "write: �ʿ� �Ķ���� lba(0~99 ��), data(32bit 16���� ��) - ssd�� lba cell�� data�� �����Ѵ�.\n";
		cout << "fullwrite: �ʿ� �Ķ���� data(32bit 16���� ��) - ssd�� ��� cell�� data�� �����Ѵ�.\n";
		cout << "read: �ʿ� �Ķ���� lba(0~99 ��) - ssd�� lba cell�� ���� �о�´�.\n";
		cout << "fullread: �ʿ� �Ķ���� NONE - ssd�� ��� cell�� ���� �о�´�\n";
		cout << "help: �ʿ� �Ķ���� NONE - ssd�� �ʿ��� command �� ��� ������ ���� ������ ��û�Ѵ�\n";
		cout << "exit: �ʿ� �Ķ���� NONE - �Ϸ��� �� ������ �ٷ� �����Ѵ�\n" << endl;

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

