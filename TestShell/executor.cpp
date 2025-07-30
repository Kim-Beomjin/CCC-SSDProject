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
		cout << "[Write] Done\n";

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
		cout << "[Read] Done\n";
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
	cout << this->HELP_DESCRIPTION;

	return true;
}

bool Exiter::execute(const string& command, LBA lba, DATA data, ISsdApp *app)
{
	return true;
}

