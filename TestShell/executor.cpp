#include <iostream>
#include "executor.h"

using namespace std;

IExecutor* ExecutorFactory::createExecutor(const string command)
{
	if (command == "write" || command == "fullwrite") return new Writer();
	if (command == "read" || command == "fullread") return new Reader();
	if (command == "help") return new Helper();
	if (command == "exit") return new Exiter();

	return nullptr;
}

bool Writer::execute(const string& command, LBA lba, DATA data, ISsdApp * app)
{
	if (command == "write")
	{
		app->Write(lba, data);
		return true;
	}
	else if (command == "fullwrite")
	{
		for (LBA lba = 0; lba < SSD_MAX_SIZE; ++lba) app->Write(lba, data);
		return true;
	}
	return false;
}

bool Reader::execute(const string& command, LBA lba, DATA data, ISsdApp * app)
{
	if (command == "read")
	{
		app->Read(lba);
		return true;
	}
	else if (command == "fullread")
	{
		for (DATA lba = 0; lba < SSD_MAX_SIZE; ++lba) app->Read(lba);
		return true;
	}
	return false;
}

bool Helper::execute(const string& command, LBA lba, DATA data, ISsdApp *app)
{
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

bool Exiter::execute(const string& command, LBA lba, DATA data, ISsdApp *app)
{
	cout << "Exit!!" << endl;
	return true;
}

