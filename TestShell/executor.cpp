#include <iostream>
#include <exception>
#include "interface.h"

using std::string;
using std::cout;
using std::endl;

class MockSsdApp;

class SsdApp : public ISsdApp {
public:
	u32 Read(u32 lba) override {
		// TODO: need to execute existing exe file of ssd
	}

	bool Write(u32 lba, u32 data) override {
		// TODO: need to execute existing exe file of ssd
	}

	static SsdApp* getInstance() {
		static SsdApp instance;
		return &instance;
	}
private:
	SsdApp() {}
};

class Writer : public IExecutor {
public:
	bool execute(const string& command, u32 lba, u32 data, ISsdApp * app) override {
		if (command == "write") {
			app->Write(lba, data);
			return true;
		}
		else if (command == "fullwrite") {
			for (u32 lba = 0; lba < SSD_MAX_SIZE; ++lba) app->Write(lba, data);
			return true;
		}
		return false;
	}
};

class Reader : public IExecutor {
public:
	bool execute(const string& command, u32 lba, u32 data, ISsdApp * app) override {
		if (command == "read") {
			app->Read(lba);
			return true;
		}
		else if (command == "fullread") {
			for (u32 lba = 0; lba < SSD_MAX_SIZE; ++lba) app->Read(lba);
			return true;
		}
		return false;
	}
};

class Helper : public IExecutor {
public:
	bool execute(const string& command, u32 lba, u32 data, ISsdApp *app) override {
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
	bool execute(const string& command, u32 lba, u32 data, ISsdApp *app) override {
		cout << "Exit!!" << endl;
		return true;
	}
};

IExecutor* ExecutorFactory::createExecutor(const string command) {
	if (command == "write" || command == "fullwrite") {
		return new Writer();
	}
	else if (command == "read" || command == "fullread") {
		return new Reader();
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
