#include <string>
#include <iostream>

#define interface struct

using std::string;
using std::cout;
using std::endl;

interface IExecutor {
	virtual bool execute(const string command = "", int lba = 0, int data = 0) = 0;
};

class Writer : public IExecutor {
public:
	bool execute(const string command = "", int lba = 0, int data = 0) override {
		if (command == "write") {

		}
		else if (command == "fullwrite") {
			
		}
		return false;
	}
};

class Reader : public IExecutor {
public:
	bool execute(const string command = "", int lba = 0, int data = 0) override {
		if (command == "read") {

		}
		else if (command == "fullread") {

		}
		return false;
	}
};

class Helper : public IExecutor {
public:
	bool execute(const string command = "", int lba = 0, int data = 0) override {
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
	bool execute(const string command = "", int lba = 0, int data = 0) override {
		return true;
	}
};

class ExecutorFactory {
public:
	IExecutor* createExecutor(const std::string command) {
		if (command == "write" || command == "fullwrite") {
			return new Writer();
		} else if (command == "read" || command == "fullread") {
			return new Reader();
		} else if (command == "help") {
			return new Helper();
		} else if (command == "exit") {
			return new Exiter();
		}
		return nullptr;
	}
};
